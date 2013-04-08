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

#ifndef __NETWORK_H__
#define __NETWORK_H__

#include "base.h"
#include <string>
#include <netdb.h>
#include <sys/socket.h>
#include <stdlib.h>

/**
 * Network utility functions.
 */
namespace HPHP { namespace Util {
///////////////////////////////////////////////////////////////////////////////
// thread-safe network functions

struct hostent* hostent_dup(const struct hostent *hostent);

class HostEnt {
public:
  HostEnt() : tmphstbuf(NULL), hstbuflen(0), index(0), n_addrs(0) {}
  ~HostEnt() { if (tmphstbuf) free(tmphstbuf); }

  void rotate_index() {
    if (++index >= n_addrs) index = 0;
  }

  void get_current_address(struct in_addr *in) const {
    memcpy(&(in->s_addr), hostbuf.h_addr_list[index], hostbuf.h_length);
  }

  struct hostent hostbuf;
  char *tmphstbuf;
  size_t hstbuflen;
  int herr;
  // Index to rotate through all available adresses
  int index, n_addrs;
};

bool safe_gethostbyname(const char *address, HostEnt &result);
std::string safe_inet_ntoa(struct in_addr &in);

///////////////////////////////////////////////////////////////////////////////
/**
 * Get local machine's primary IP address.
 */
std::string GetPrimaryIP();

/**
 * Get network bytes per second.
 */
bool GetNetworkStats(const char *iface, int &in_bps, int &out_bps);

///////////////////////////////////////////////////////////////////////////////
}}

#endif // __NETWORK_H__
