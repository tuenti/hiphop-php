/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010- Facebook, Inc. (http://www.facebook.com)         |
   | Copyright (c) 1997-2010 The PHP Group                                |
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

#include <runtime/ext/ext_memcachepool.h>
#include <runtime/base/util/request_local.h>
#include <runtime/base/ini_setting.h>
#include <runtime/ext/ext_function.h>
#include <runtime/ext/ext_error.h>
#include <util/logger.h>
#include <runtime/ext/ext_zlib.h>

namespace HPHP {
IMPLEMENT_DEFAULT_EXTENSION(memcachepool);

/* use lowest byte for flags */
const int64 k_MEMCACHE_SERIALIZED = 1;
const int64 k_MEMCACHE_COMPRESSED = 2;

/* use second lowest byte to indicate data type */
const int64 MMC_TYPE_BOOL   = 0x0100;
const int64 MMC_TYPE_LONG   = 0x0300;
const int64 MMC_TYPE_DOUBLE = 0x0700;


class MemcacheObjectData;

struct StorageData {
  time_t last_config_update;
  bool persistent;
  c_MemcachePool * memcachepool_object;
};

class MemcachePoolRequest: public RequestEventHandler {
  public:
    // Static map per thread to contain per-instante data
    std::map<int, StorageData> storage_map;
    std::map<c_MemcachePool *, MemcacheObjectData *> obj_map;

    virtual void requestInit() {
    }

    virtual void requestShutdown() {
      // This is done to close all TCP connections because libmemcached use
      // persistent connections which could not afford
      std::map<int, StorageData>::iterator it;

      for (it = storage_map.begin(); it != storage_map.end(); it++) {
        if (! (*it).second.persistent) {
          (*it).second.memcachepool_object->t_close();
        }
      }
    }
};

IMPLEMENT_STATIC_REQUEST_LOCAL(MemcachePoolRequest, s_memcachepool_data);

// Macros to access to global an per-instante variables
#define MEMCACHEG(name) s_memcachepool_data->name
#define MEMCACHEL(name) s_memcachepool_data->obj_map[this]->name

class MemcacheObjectData {
  public:
  memcached_st *tcp_st;
  memcached_st *udp_st;
  int compress_threshold;
  int min_compress_savings;
  Variant failure_callback;

  MemcacheObjectData() {
    tcp_st = memcached_create(NULL);
    udp_st = memcached_create(NULL);
    min_compress_savings = 0.2;
    failure_callback.setNull();

    compress_threshold = RuntimeOption::MemcachePoolCompressThreshold;

    memcached_behavior_set(tcp_st, MEMCACHED_BEHAVIOR_DISTRIBUTION, RuntimeOption::MemcachePoolHashStrategy);
    memcached_behavior_set(udp_st, MEMCACHED_BEHAVIOR_DISTRIBUTION, RuntimeOption::MemcachePoolHashStrategy);
    memcached_behavior_set(tcp_st, MEMCACHED_BEHAVIOR_HASH, RuntimeOption::MemcachePoolHashFunction);
    memcached_behavior_set(udp_st, MEMCACHED_BEHAVIOR_HASH, RuntimeOption::MemcachePoolHashFunction);

    memcached_behavior_set(tcp_st, MEMCACHED_BEHAVIOR_BINARY_PROTOCOL, 1);

    memcached_behavior_set(udp_st, MEMCACHED_BEHAVIOR_USE_UDP, 1);
    memcached_behavior_set(udp_st, MEMCACHED_BEHAVIOR_BINARY_PROTOCOL, 1);
    memcached_behavior_set(udp_st, MEMCACHED_BEHAVIOR_CHECK_OPAQUE, 1);
    memcached_behavior_set(udp_st, MEMCACHED_BEHAVIOR_NOREPLY, 0);
    memcached_behavior_set(udp_st, MEMCACHED_BEHAVIOR_NO_BLOCK, 1);
    memcached_behavior_set(udp_st, MEMCACHED_BEHAVIOR_UDP_ALWAYS_FLUSH, 1);
  }

  ~MemcacheObjectData() {
    memcached_free(tcp_st);
    memcached_free(udp_st);
  }
};

///////////////////////////////////////////////////////////////////////////////
// methods
   
Object c_MemcachePool::ti_getstoragememcache(const char *, int storage_id, int timestamp, 
                                             VRefParam empty, bool persistent) {
  STATIC_METHOD_INJECTION_BUILTIN(MemcachePool, MemcachePool::getstoragememcache);
  StorageData *st_data;

  empty = false;

  if (MEMCACHEG(storage_map).count(storage_id) < 1) {
    if (RuntimeOption::MemcachePoolDebug) {
      Logger::Verbose("[MemcachePool] Creating new MemcachePool object for storage id  %d", storage_id);
    }
    // Insert new entry
    st_data = &(MEMCACHEG(storage_map)[storage_id]);
    st_data->last_config_update = timestamp;
    st_data->persistent = persistent;
    st_data->memcachepool_object = NEWOBJ(c_MemcachePool);
    // We don't want this object to be destroyed when it gets unreferenced on PHP
    st_data->memcachepool_object->incRefCount();

    MEMCACHEG(obj_map)[st_data->memcachepool_object] = new MemcacheObjectData;

    empty = true;
  } else {
    // Get old entry
    st_data = &(MEMCACHEG(storage_map)[storage_id]);
  }

  if (st_data->last_config_update < timestamp) {
    if (RuntimeOption::MemcachePoolDebug) {
      Logger::Verbose("[MemcachePool] Object too old for storage id  %d, destroying data", storage_id);
    }
    delete MEMCACHEG(obj_map)[st_data->memcachepool_object];
    MEMCACHEG(obj_map)[st_data->memcachepool_object] = new MemcacheObjectData;

    st_data->last_config_update = timestamp;
    st_data->persistent = persistent;

    empty = true;
  } 

  return st_data->memcachepool_object;
}

c_MemcachePool::c_MemcachePool(const ObjectStaticCallbacks *cb) : ExtObjectData(cb) {
  MEMCACHEG(obj_map)[this] = NULL;
  if (! s_memcachepool_data->obj_map[this]) {
    s_memcachepool_data->obj_map[this] = new MemcacheObjectData;
  }
}

c_MemcachePool::~c_MemcachePool() {
  if (RuntimeOption::MemcachePoolDebug) {
    Logger::Verbose("[MemcachePool] Destroying MemcachePool object %p", this);
  }

  if (MEMCACHEG(storage_map).size() > 0) {
      std::map<int, StorageData>::iterator it;

      for (it = MEMCACHEG(storage_map).begin(); it != MEMCACHEG(storage_map).end(); it++) {
        if (this == (*it).second.memcachepool_object) {
            break;
        }
      }

      MEMCACHEG(storage_map).erase(it);
  }

  MEMCACHEG(obj_map).erase(this);
}

void c_MemcachePool::t___construct() {
  INSTANCE_METHOD_INJECTION_BUILTIN(MemcachePool, MemcachePool::__construct);
  return;
}

bool c_MemcachePool::t_connect(CStrRef host, int port /*= 0*/,
                           int timeout /*= 0*/,
                           int timeoutms /*= 0*/) {
  INSTANCE_METHOD_INJECTION_BUILTIN(MemcachePool, MemcachePool::connect);
  memcached_return_t ret;

  if (!host.empty() && host[0] == '/') {
    ret = memcached_server_add_unix_socket(MEMCACHEL(tcp_st), host.c_str());
  } else {
    ret = memcached_server_add(MEMCACHEL(tcp_st), host.c_str(), port);
  }

  return check_memcache_return(MEMCACHEL(tcp_st), ret, "", "Error adding server");
}

bool c_MemcachePool::t_pconnect(CStrRef host, int port /*= 0*/,
                            int timeout /*= 0*/,
                            int timeoutms /*= 0*/) {
  INSTANCE_METHOD_INJECTION_BUILTIN(MemcachePool, MemcachePool::pconnect);
  return t_connect(host, port, timeout, timeoutms);
}

String static memcache_prepare_for_storage(CVarRef var, int &flag, int threshold) {
  String serialized;

  if (flag & 0xffff & ~k_MEMCACHE_COMPRESSED) {
    raise_warning("The lowest two bytes of the flags array are reserved for memcache internal use.");
    flag &= k_MEMCACHE_COMPRESSED & ~0xffff;
  }

  if (var.isString() || var.isNumeric() || var.isBoolean()) {
    if (var.isInteger()) {
      flag |= MMC_TYPE_LONG;
    } else if (var.isBoolean()) {
      flag |= MMC_TYPE_BOOL;
      serialized = var ? "1" : "0";
    } else if (var.isDouble()) {
      flag |= MMC_TYPE_DOUBLE;
    }

    if (!var.isBoolean()) {
      serialized = var.toString();
    }
  } else {
    flag |= k_MEMCACHE_SERIALIZED;
    serialized = f_serialize(var);
  }

  if (serialized.length() >= threshold) {
    Variant compressed = f_gzcompress(serialized);

    if (compressed) {
        flag |= k_MEMCACHE_COMPRESSED;
        serialized = compressed.toString();
    }
  }

  return serialized;
}

Variant static memcache_fetch_from_storage(const char *payload,
                                           size_t payload_len,
                                           uint32_t flags) {
  Variant ret = null;

  if (flags & k_MEMCACHE_COMPRESSED) {
    ret = f_gzuncompress(String(payload, payload_len, CopyString));
  } else {
    ret = String(payload, payload_len, CopyString);
  }

  if (flags & k_MEMCACHE_SERIALIZED) {
    ret = f_unserialize(ret);
  } switch (flags & 0x0f00) {
    case MMC_TYPE_LONG:
      return ret.toInt64();
    case MMC_TYPE_DOUBLE:
      return ret.toDouble();
    case MMC_TYPE_BOOL:
      return ret.toBoolean();
  }

  return ret;
}

bool c_MemcachePool::t_add(CStrRef key, CVarRef var, int flag /*= 0*/,
                       int expire /*= 0*/) {
  INSTANCE_METHOD_INJECTION_BUILTIN(MemcachePool, MemcachePool::add);
  if (key.empty()) {
    raise_warning("Key cannot be empty");
    return false;
  }

  String serialized = memcache_prepare_for_storage(var, flag, MEMCACHEL(compress_threshold));

  memcached_return_t ret = memcached_add(MEMCACHEL(tcp_st),
                                        key.c_str(), key.length(),
                                        serialized.c_str(),
                                        serialized.length(),
                                        expire, flag);

  return check_memcache_return(MEMCACHEL(tcp_st), ret, key);
}

bool c_MemcachePool::t_set(CStrRef key, CVarRef var, int flag /*= 0*/,
                       int expire /*= 0*/) {
  INSTANCE_METHOD_INJECTION_BUILTIN(MemcachePool, MemcachePool::set);
  if (key.empty()) {
    raise_warning("Key cannot be empty");
    return false;
  }

  String serialized = memcache_prepare_for_storage(var, flag, MEMCACHEL(compress_threshold));

  memcached_return_t ret = memcached_set(MEMCACHEL(tcp_st),
                                        key.c_str(), key.length(),
                                        serialized.c_str(),
                                        serialized.length(),
                                        expire, flag);

  return check_memcache_return(MEMCACHEL(tcp_st), ret, key);
}

bool c_MemcachePool::t_cas(CStrRef key, CVarRef var, int flag,
                       int expire, double cas) {
  INSTANCE_METHOD_INJECTION_BUILTIN(MemcachePool, MemcachePool::cas);
  if (key.empty()) {
    raise_warning("Key cannot be empty");
    return false;
  }

  String serialized = memcache_prepare_for_storage(var, flag, MEMCACHEL(compress_threshold));

  memcached_return_t ret = memcached_cas(MEMCACHEL(tcp_st),
                                        key.c_str(), key.length(),
                                        serialized.c_str(),
                                        serialized.length(),
                                        expire, flag, cas);

  return check_memcache_return(MEMCACHEL(tcp_st), ret, key);
}

bool c_MemcachePool::t_replace(CStrRef key, CVarRef var, int flag /*= 0*/,
                           int expire /*= 0*/) {
  INSTANCE_METHOD_INJECTION_BUILTIN(MemcachePool, MemcachePool::replace);
  if (key.empty()) {
    raise_warning("Key cannot be empty");
    return false;
  }

  String serialized = memcache_prepare_for_storage(var, flag, MEMCACHEL(compress_threshold));

  memcached_return_t ret = memcached_replace(MEMCACHEL(tcp_st),
                                             key.c_str(), key.length(),
                                             serialized.c_str(),
                                             serialized.length(),
                                             expire, flag);


  return check_memcache_return(MEMCACHEL(tcp_st), ret, key);
}

Variant c_MemcachePool::t_get(CVarRef key, VRefParam flags /*= null*/, VRefParam cas /*= null*/) {
  INSTANCE_METHOD_INJECTION_BUILTIN(MemcachePool, MemcachePool::get);
  TAINT_OBSERVER(TAINT_BIT_ALL, TAINT_BIT_NONE);

  // Use UDP if we have any server available
  memcached_st *memc = MEMCACHEL(tcp_st);
  if (memcached_server_count(MEMCACHEL(udp_st)) > 0)
    memc = MEMCACHEL(udp_st);

  std::vector<const char *> real_keys;
  std::vector<size_t> key_len;
  Array keyArr;

  if (key.is(KindOfArray)) {
    keyArr = key.toArray();

    if (cas.isReferenced())
      cas = Array();
    if (flags.isReferenced())
      flags = Array();
  }
  else {
    keyArr = Array(key);
  }

  real_keys.reserve(keyArr.size());
  key_len.reserve(keyArr.size());

  for (ArrayIter iter(keyArr); iter; ++iter) {
    real_keys.push_back(const_cast<char *>(iter.second().toString().c_str()));
    key_len.push_back(iter.second().toString().length());
  }

  const char *payload, *res_key;
  size_t payload_len, res_key_len;
  int64 cflags, ccas;
  memcached_result_st result;
  String curkey;

  Array return_val = Array::Create();

  if (real_keys.empty())
    return return_val;

  memcached_return_t ret = memcached_mget(memc, &real_keys[0], &key_len[0], 
                                          real_keys.size());

  if (ret != MEMCACHED_SUCCESS) {
    return check_memcache_return(memc, ret, "", "Error doing multiget");
  }

  memcached_result_create(memc, &result);

  while ((memcached_fetch_result(memc, &result, &ret)) != NULL) {
    if (ret != MEMCACHED_SUCCESS) {
      check_memcache_return(memc, ret, "", "Error getting multiget results");
      continue;
    }

    payload     = memcached_result_value(&result);
    payload_len = memcached_result_length(&result);
    res_key     = memcached_result_key_value(&result);
    ccas        = memcached_result_cas(&result);
    cflags      = memcached_result_flags(&result);
    res_key_len = memcached_result_key_length(&result);

    curkey = String(res_key, res_key_len, CopyString);

    return_val.set(curkey, memcache_fetch_from_storage(payload, payload_len, cflags));

    if (flags.isReferenced()) {
      if (key.is(KindOfArray))
        flags->set(curkey, cflags, true);
      else
        flags = cflags;
    }
        
    if (cas.isReferenced()) {
      if (key.is(KindOfArray))
        cas->set(curkey, ccas, true);
      else
        cas = ccas;
    }
  }
  if ((ret != MEMCACHED_END) && (ret != MEMCACHED_NOTFOUND)) {
    check_memcache_return(memc, ret, "", "Error getting multiget results");
  }

  memcached_result_free(&result);

  if (key.is(KindOfArray))
    return return_val;
  else {
    if (return_val[curkey].is(KindOfNull))
        return false;
    return return_val[curkey];
  }
}

bool c_MemcachePool::t_delete(CStrRef key, int expire /*= 0*/) {
  INSTANCE_METHOD_INJECTION_BUILTIN(MemcachePool, MemcachePool::delete);
  if (key.empty()) {
    raise_warning("Key cannot be empty");
    return false;
  }

  memcached_return_t ret = memcached_delete(MEMCACHEL(tcp_st),
                                            key.c_str(), key.length(),
                                            expire);
  return check_memcache_return(MEMCACHEL(tcp_st), ret, key);
}

Variant c_MemcachePool::t_increment(CStrRef key, int offset /*= 1*/) {
  INSTANCE_METHOD_INJECTION_BUILTIN(MemcachePool, MemcachePool::increment);
  if (key.empty()) {
    raise_warning("Key cannot be empty");
    return false;
  }

  uint64_t value;
  memcached_return_t ret = memcached_increment(MEMCACHEL(tcp_st), key.c_str(),
                                              key.length(), offset, &value);

  if (ret == MEMCACHED_SUCCESS) {
    return (int64)value;
  }

  return check_memcache_return(MEMCACHEL(tcp_st), ret, key);
}

Variant c_MemcachePool::t_decrement(CStrRef key, int offset /*= 1*/) {
  INSTANCE_METHOD_INJECTION_BUILTIN(MemcachePool, MemcachePool::decrement);
  if (key.empty()) {
    raise_warning("Key cannot be empty");
    return false;
  }

  uint64_t value;
  memcached_return_t ret = memcached_decrement(MEMCACHEL(tcp_st), key.c_str(),
                                              key.length(), offset, &value);
  if (ret == MEMCACHED_SUCCESS) {
    return (int64)value;
  }

  return check_memcache_return(MEMCACHEL(tcp_st), ret, key);
}

bool c_MemcachePool::t_close() {
  INSTANCE_METHOD_INJECTION_BUILTIN(MemcachePool, MemcachePool::close);
  memcached_quit(MEMCACHEL(tcp_st));
  memcached_quit(MEMCACHEL(udp_st));
  return true;
}

Variant c_MemcachePool::t_getversion() {
  INSTANCE_METHOD_INJECTION_BUILTIN(MemcachePool, MemcachePool::getversion);
  int server_count = memcached_server_count(MEMCACHEL(tcp_st));
  char version[16];
  int version_len = 0;

  memcached_return_t ret = memcached_version(MEMCACHEL(tcp_st));

  if (ret != MEMCACHED_SUCCESS) {
    check_memcache_return(MEMCACHEL(tcp_st), ret, "", "Error getting memcached version");
    return false;
  }

  for (int x = 0; x < server_count; x++) {
    memcached_server_instance_st instance =
      memcached_server_instance_by_position(MEMCACHEL(tcp_st), x);

    if (!instance->major_version) {
      continue;
    }

    version_len = snprintf(version, sizeof(version), "%d.%d.%d",
        instance->major_version, instance->minor_version,
        instance->micro_version);
    return String(version, version_len, CopyString);
  }

  return false;
}

bool c_MemcachePool::t_flush(int expire /*= 0*/) {
  INSTANCE_METHOD_INJECTION_BUILTIN(MemcachePool, MemcachePool::flush);
  memcached_return_t ret = memcached_flush(MEMCACHEL(tcp_st), expire);
    
  return check_memcache_return(MEMCACHEL(tcp_st), ret, "", "Error flushing servers");
}

bool c_MemcachePool::t_setoptimeout(int64 timeoutms) {
  INSTANCE_METHOD_INJECTION_BUILTIN(MemcachePool, MemcachePool::setoptimeout);
  memcached_behavior_set(MEMCACHEL(tcp_st), MEMCACHED_BEHAVIOR_CONNECT_TIMEOUT, timeoutms);
  memcached_behavior_set(MEMCACHEL(udp_st), MEMCACHED_BEHAVIOR_CONNECT_TIMEOUT, timeoutms);
  memcached_behavior_set(MEMCACHEL(tcp_st), MEMCACHED_BEHAVIOR_POLL_TIMEOUT, timeoutms);
  memcached_behavior_set(MEMCACHEL(udp_st), MEMCACHED_BEHAVIOR_POLL_TIMEOUT, timeoutms);

  return true;
}

int c_MemcachePool::t_getserverstatus(CStrRef host, int port /* = 0 */) {
  INSTANCE_METHOD_INJECTION_BUILTIN(MemcachePool, MemcachePool::getserverstatus);
  /* intentionally doing nothing for now */
  return 1;
}

bool c_MemcachePool::t_setcompressthreshold(int threshold,
                                        double min_savings /* = 0.2 */) {
  INSTANCE_METHOD_INJECTION_BUILTIN(MemcachePool, MemcachePool::setcompressthreshold);
  if (threshold < 0) {
    raise_warning("threshold must be a positive integer");
    return false;
  }

  if (min_savings < 0 || min_savings > 1) {
    raise_warning("min_savings must be a float in the 0..1 range");
    return false;
  }

  MEMCACHEL(compress_threshold) = threshold;
  MEMCACHEL(min_compress_savings) = min_savings;

  return true;
}

Array static memcache_build_stats(const memcached_st *ptr,
                                memcached_stat_st *memc_stat,
                                memcached_return_t *ret) {
  char **curr_key;
  char **stat_keys = memcached_stat_get_keys(ptr, memc_stat, ret);

  if (*ret != MEMCACHED_SUCCESS) {
    if (stat_keys) {
      free(stat_keys);
    }
    return NULL;
  }

  Array return_val = Array::Create();

  for (curr_key = stat_keys; *curr_key; curr_key++) {
    char *mc_val;
    mc_val = memcached_stat_get_value(ptr, memc_stat, *curr_key, ret);
    if (*ret != MEMCACHED_SUCCESS) {
      break;
    }
    return_val.set(String(*curr_key, CopyString),
                   String(mc_val, CopyString));
    free(mc_val);
  }

  free(stat_keys);
  return return_val;
}


Array c_MemcachePool::t_getstats(CStrRef type /* = null_string */,
                             int slabid /* = 0 */, int limit /* = 100 */) {
  INSTANCE_METHOD_INJECTION_BUILTIN(MemcachePool, MemcachePool::getstats);
  if (!memcached_server_count(MEMCACHEL(tcp_st))) {
    return NULL;
  }

  char extra_args[30] = {0};

  if (slabid) {
    snprintf(extra_args, sizeof(extra_args), "%s %d %d", type.c_str(),
             slabid, limit);
  } else if (!type.empty()) {
    snprintf(extra_args, sizeof(extra_args), "%s", type.c_str());
  }

  memcached_server_instance_st instance =
    memcached_server_instance_by_position(MEMCACHEL(tcp_st), 0);

  memcached_stat_st stats;
  memcached_return_t ret;
  ret = memcached_stat_servername(&stats, extra_args, instance->hostname, 
                                  instance->port);
  if (ret != MEMCACHED_SUCCESS) {
    check_memcache_return(MEMCACHEL(tcp_st), ret, "", "Error getting stats from server");
    return NULL;
  }

  return memcache_build_stats(MEMCACHEL(tcp_st), &stats, &ret);
}

Array c_MemcachePool::t_getextendedstats(CStrRef type /* = null_string */,
                                     int slabid /* = 0 */,
                                     int limit /* = 100 */) {
  INSTANCE_METHOD_INJECTION_BUILTIN(MemcachePool, MemcachePool::getextendedstats);
  memcached_return_t ret;
  memcached_stat_st *stats;

  stats = memcached_stat(MEMCACHEL(tcp_st), NULL, &ret);
  if (ret != MEMCACHED_SUCCESS) {
    check_memcache_return(MEMCACHEL(tcp_st), ret, "", "Error getting memcache stats");
    return NULL;
  }

  int server_count = memcached_server_count(MEMCACHEL(tcp_st));

  Array return_val;

  for (int server_id = 0; server_id < server_count; server_id++) {
    memcached_server_instance_st server;
    memcached_stat_st *stat;
    char stats_key[30] = {0};
    size_t key_len;

    server = memcached_server_instance_by_position(MEMCACHEL(tcp_st), server_id);
    stat = stats + server_id;

    Array server_stats = memcache_build_stats(MEMCACHEL(tcp_st), stat, &ret);
    if (ret != MEMCACHED_SUCCESS) {
      continue;
    }

    key_len = snprintf(stats_key, sizeof(stats_key),
                       "%s:%d", server->hostname, server->port);

    return_val.set(String(stats_key, key_len, CopyString), server_stats);
  }

  free(stats);
  return return_val;
}

bool c_MemcachePool::t_setserverparams(CStrRef host, int port /* = 11211 */,
                                   int timeout /* = 0 */,
                                   int retry_interval /* = 0 */,
                                   bool status /* = true */) {
  INSTANCE_METHOD_INJECTION_BUILTIN(MemcachePool, MemcachePool::setserverparams);
  t_setoptimeout(1000 * timeout);

  memcached_behavior_set(MEMCACHEL(tcp_st), MEMCACHED_BEHAVIOR_RETRY_TIMEOUT, retry_interval);
  memcached_behavior_set(MEMCACHEL(udp_st), MEMCACHED_BEHAVIOR_RETRY_TIMEOUT, retry_interval);

  return true;
}

/* 
 * Call to the user defined callback. Callback function have this prototype:
 * function mycallback($host, $tcp_port, $udp_port, $error, $errnum)
 *
 */
void c_MemcachePool::exec_failure_callback(const char * hostname, int tcp_port, int udp_port,
                                          memcached_return_t ret, const char * error, Array backtrace) {
  if (MEMCACHEL(failure_callback).isNull())
    return;

  Array params(ArrayInit(5).set(hostname).set(tcp_port).set(udp_port).set(ret).set(error).set(backtrace));

  f_call_user_func_array(MEMCACHEL(failure_callback), params);
}

/* 
 * Check memcache return value and obtain further information on error if
 * possible. Also returns a boolean with the exit status, in order to use
 * this function return value on API functions
 *
 */
bool c_MemcachePool::check_memcache_return(memcached_st * st,
                                           memcached_return_t ret,
                                           String key,  /* = "" */
                                           char * default_msg /* = "" */) {
  memcached_server_instance_st instance;
  const char * str_error=NULL, *hostname="";
  int tcp_port=0, udp_port=0;

  if (ret == MEMCACHED_SUCCESS)
    return true;

  if ((ret == MEMCACHED_NOTSTORED)
      || (ret == MEMCACHED_DATA_EXISTS)
      || (ret == MEMCACHED_NOTFOUND))
    return false;

  if (MEMCACHEL(failure_callback).isNull())
    return false;

  if (ret == MEMCACHED_ERRNO) {
    if (st->cached_errno != 0)
      str_error = strerror(st->cached_errno);
    else
      str_error = strerror(errno);
  }

  if (key.length() > 1) {
    // If we have a key we can try to get the server used
    memcached_return_t ret2;

    instance = memcached_server_by_key(st, key.c_str(), key.length(), &ret2);
    if (instance) {
      hostname = instance->hostname;

      if (memcached_behavior_get(st, MEMCACHED_BEHAVIOR_USE_UDP))
        udp_port = instance->port;
      else
        tcp_port = instance->port;
    }

    if (ret != MEMCACHED_ERRNO)
      str_error = memcached_server_error(instance);
  }

  if (!str_error)
    str_error = memcached_strerror(st, ret);

  Array backtrace = f_debug_backtrace(false);
  exec_failure_callback(hostname, tcp_port, udp_port, ret, str_error, backtrace);

  return false;
}

bool c_MemcachePool::t_setfailurecallback(CVarRef failure_callback) {
  INSTANCE_METHOD_INJECTION_BUILTIN(MemcachePool, MemcachePool::setfailurecallback);
  if (!f_is_callable(failure_callback)) {
    raise_warning("Not a valid callback function %s", failure_callback.toString().data());
    return false;
  } 

  MEMCACHEL(failure_callback) = failure_callback;

  return true;
}

bool c_MemcachePool::t_addserver(CStrRef host, int tcp_port, int udp_port, 
                             bool persistent, int weight, int timeout, 
                             int retry_interval, bool status) {
  INSTANCE_METHOD_INJECTION_BUILTIN(MemcachePool, MemcachePool::addserver);
  memcached_return_t ret  = MEMCACHED_SUCCESS;
  memcached_return_t ret2 = MEMCACHED_SUCCESS;
  int port = 0;

  if (tcp_port > 0) {
    port = tcp_port;
    ret = memcached_server_add_with_weight(MEMCACHEL(tcp_st), host.c_str(),
                                             tcp_port, weight);
    check_memcache_return(MEMCACHEL(tcp_st), ret, "", "Cannot add server");
  }

  if (udp_port > 0) {
    port = udp_port;
    ret2 = memcached_server_add_udp_with_weight(MEMCACHEL(udp_st), host.c_str(),
                                              udp_port, weight);

    check_memcache_return(MEMCACHEL(udp_st), ret, "", "Cannot add server");
  }

  if (!port) {
    //Socket unix or invalid parameters
    String unix_sock = host.replace("unix://","");

    if (unix_sock.empty())
      return false;

    ret = memcached_server_add_unix_socket_with_weight(MEMCACHEL(tcp_st),
                                                       unix_sock.c_str(), weight);
    check_memcache_return(MEMCACHEL(udp_st), ret, "", "Cannot add server");
  }

  if ((ret == MEMCACHED_SUCCESS) && (ret2 == MEMCACHED_SUCCESS)) {
    if (port) t_setserverparams(host, port, timeout, retry_interval, status);
    return true;
  }

  return false;
}

Variant c_MemcachePool::t___destruct() {
  INSTANCE_METHOD_INJECTION_BUILTIN(MemcachePool, MemcachePool::__destruct);
  t_close();
  return null;
}

///////////////////////////////////////////////////////////////////////////////
}
