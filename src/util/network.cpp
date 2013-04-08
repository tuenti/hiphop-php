/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010- Facebook, Inc. (http://www.facebook.com)         |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/
#include "network.h"
#include "lock.h"
#include "process.h"
#include "util.h"

#include <netinet/in.h>
#include <arpa/nameser.h>
#include <resolv.h>
#include <util/thread_local.h>
#include <util/ares.h>

#ifdef CARES_FOUND
#include <ares.h>
#endif

using namespace std;

namespace HPHP {
///////////////////////////////////////////////////////////////////////////////
// without calling res_init(), any call to getaddrinfo() may leak memory:
//  http://sources.redhat.com/ml/libc-hacker/2004-02/msg00049.html

class ResolverLibInitializer {
public:
  ResolverLibInitializer() {
    res_init();
#ifdef CARES_FOUND
    ares_library_init(ARES_LIB_INIT_ALL);
#endif
  }
};
static ResolverLibInitializer _resolver_lib_initializer;
///////////////////////////////////////////////////////////////////////////////
// thread-safe network functions

std::string Util::safe_inet_ntoa(struct in_addr &in) {
  char buf[256];
  memset(buf, 0, sizeof(buf));
  inet_ntop(AF_INET, &in, buf, sizeof(buf)-1);
  return buf;
}

#ifdef CARES_FOUND
class NetworkData {
public:
  ares_channel aresChannel;
  NetworkData() {
    ares_init(&aresChannel);
  }
};
static IMPLEMENT_THREAD_LOCAL(NetworkData, s_networkData);
#endif

struct hostent* Util::hostent_dup(const struct hostent *hostent) {
  struct hostent* result;
  size_t len, name_len;
  int n_aliases = 0;
  int n_addrs = 0;
  len = sizeof(struct hostent);
  name_len = strlen(hostent->h_name);
  len += name_len + 1;
  len += 2 * sizeof(char*);
  for(char** alias = hostent->h_aliases; *alias; alias++) {
    len += sizeof(char*) + strlen(*alias) + 1;
    n_aliases++;
  }
  for(char** addr = hostent->h_addr_list; *addr; addr++) {
    len += sizeof(char*) + hostent->h_length;
    n_addrs++;
  }
  result = reinterpret_cast<struct hostent*>(malloc(sizeof(struct hostent) + len));
  result->h_addrtype = hostent->h_addrtype;
  result->h_length = hostent->h_length;
  result->h_aliases = reinterpret_cast<char**>(result + 1);
  result->h_addr_list = result->h_aliases + n_aliases + 1;

  char* p = reinterpret_cast<char*>(result->h_addr_list + n_addrs + 1);
  for(char **alias = hostent->h_aliases, **new_alias = result->h_aliases; *alias; alias++, new_alias++) {
    size_t l = strlen(*alias) + 1;
    memcpy(p, *alias, l);
    *new_alias = p;
    p += l;
  }
  result->h_aliases[n_aliases] = 0;
  for(char **addr = hostent->h_addr_list, **new_addr = result->h_addr_list; *addr; addr++, new_addr++) {
    memcpy(p, *addr, hostent->h_length);
    *new_addr = p;
    p += hostent->h_length;
  }
  result->h_addr_list[n_addrs] = 0;
  result->h_name = p;
  memcpy(result->h_name, hostent->h_name, name_len + 1);
  ASSERT(len == (size_t)(p + name_len + 1 - reinterpret_cast<char*>(result)));
  return result;
}

#ifdef CARES_FOUND

static void ares_gethostbyname_cb(void *arg, int status, int timeouts, struct hostent *hostent) {
  Util::HostEnt& result = *reinterpret_cast<Util::HostEnt*>(arg);
  if (hostent && status == ARES_SUCCESS) {
    result.tmphstbuf = reinterpret_cast<char*>(Util::hostent_dup(hostent));
    result.hostbuf = *reinterpret_cast<struct hostent*>(result.tmphstbuf);
    result.herr = 0;
  } else {
    result.herr = status;
  }
}
#endif

bool Util::safe_gethostbyname(const char *address, HostEnt &result) {
#if defined(__APPLE__)
  struct hostent *hp = gethostbyname(address);

  if (!hp) {
    return false;
  }

  result.hostbuf = *hp;
  freehostent(hp);
  return true;
#else
#ifdef CARES_FOUND
  fd_set readers, writers;
  result.herr = 0;
  result.tmphstbuf = 0;
  int nfds, count;
  struct timeval tv, *tvp;
  ares_channel ares_ch = s_networkData->aresChannel;
  ares_gethostbyname(ares_ch, address, PF_INET, ares_gethostbyname_cb, &result);
  for (;;) {
    FD_ZERO(&readers);
    FD_ZERO(&writers);
    nfds = ares_fds(ares_ch, &readers, &writers);
    if (nfds == 0)
      break;
    if (result.herr) {
      ares_destroy(ares_ch);
      ares_init(&s_networkData->aresChannel);
      return false;
    }
    tvp = ares_timeout(ares_ch, NULL, &tv);
    count = select(nfds, &readers, &writers, NULL, tvp);
    ares_process(ares_ch, &readers, &writers);
  }
  return !result.herr;
#else
  struct hostent *hp;
  int res;

  result.hstbuflen = 1024;
  result.tmphstbuf = (char*)malloc(result.hstbuflen);
  while ((res = gethostbyname_r(address, &result.hostbuf, result.tmphstbuf,
                                result.hstbuflen, &hp, &result.herr)) == ERANGE) {
    result.hstbuflen *= 2;
    result.tmphstbuf = (char*)realloc(result.tmphstbuf, result.hstbuflen);
  }

  result.index = 0;
  result.n_addrs = 0;
  for(char** addr = result.hostbuf.h_addr_list; *addr; addr++) {
    result.n_addrs++;
  }

  return !res && hp;
#endif
#endif
}

///////////////////////////////////////////////////////////////////////////////

std::string Util::GetPrimaryIP() {
  struct utsname buf;
  uname((struct utsname *)&buf);

  HostEnt result;
  if (!safe_gethostbyname(buf.nodename, result)) {
    return buf.nodename;
  }

  struct in_addr in;
  memcpy(&in.s_addr, *(result.hostbuf.h_addr_list), sizeof(in.s_addr));
  return safe_inet_ntoa(in);
}

bool Util::GetNetworkStats(const char *iface, int &in_bps, int &out_bps) {
  ASSERT(iface && *iface);

  const char *argv[] = {"", "1", "1", "-n", "DEV", NULL};
  string out;
  Process::Exec("sar", argv, NULL, out);

  vector<string> lines;
  Util::split('\n', out.c_str(), lines, true);
  for (unsigned int i = 0; i < lines.size(); i++) {
    string &line = lines[i];
    if (line.find(iface) != string::npos) {
      vector<string> fields;
      Util::split(' ', line.c_str(), fields, true);
      if (fields[1] == iface) {
        in_bps = atoll(fields[4].c_str());
        out_bps = atoll(fields[5].c_str());
        return true;
      }
    }
  }
  return false;
}

///////////////////////////////////////////////////////////////////////////////
}
