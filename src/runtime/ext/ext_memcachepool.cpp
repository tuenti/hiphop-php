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

#define MMC_SERIALIZED 1
#define MMC_COMPRESSED 2

namespace HPHP {
IMPLEMENT_DEFAULT_EXTENSION(memcachepool);

#define getCheckedArrayRetType(input, fail, type)                       \
  Variant::TypedValueAccessor tva_##input = input.getTypedAccessor();   \
  if (UNLIKELY(Variant::GetAccessorType(tva_##input) != KindOfArray)) { \
    throw_bad_array_exception();                                        \
    return fail;                                                        \
  }                                                                     \
  type arr_##input = Variant::GetAsArray(tva_##input);

bool memcachepool_ini_on_update_hash_strategy(CStrRef value, void *p);
bool memcachepool_ini_on_update_hash_function(CStrRef value, void *p);

class MEMCACHEPOOLGlobals : public RequestEventHandler {
public:
  std::string hash_strategy;
  std::string hash_function;

  MEMCACHEPOOLGlobals() {}

  virtual void requestInit() {
    hash_strategy = "standard";
    hash_function = "crc32";

    IniSetting::Bind("memcachepool.hash_strategy", "standard",
                     memcachepool_ini_on_update_hash_strategy,  &hash_strategy);
    IniSetting::Bind("memcachepool.hash_function", "crc32",
                     memcachepool_ini_on_update_hash_function,  &hash_function);
  }

  virtual void requestShutdown() {
  }
};

IMPLEMENT_STATIC_REQUEST_LOCAL(MEMCACHEPOOLGlobals, s_memcachepool_globals);
#define MEMCACHEG(name) s_memcachepool_globals->name

bool memcachepool_ini_on_update_hash_strategy(CStrRef value, void *p) {
  if (!strncasecmp(value.data(), "standard", sizeof("standard"))) {
    MEMCACHEG(hash_strategy) = "standard";
  } else if (!strncasecmp(value.data(), "standard", sizeof("consistent"))) {
    MEMCACHEG(hash_strategy) = "consistent";
  } else {
    return false;
  }
  return true;
}

bool memcachepool_ini_on_update_hash_function(CStrRef value, void *p) {
  if (!strncasecmp(value.data(), "crc32", sizeof("crc32"))) {
    MEMCACHEG(hash_strategy) = "crc32";
  } else if (!strncasecmp(value.data(), "fnv", sizeof("fnv"))) {
    MEMCACHEG(hash_strategy) = "fnv";
  } else {
    return false;
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// methods

c_MemcachePool::c_MemcachePool() : m_memcache(), m_memcache_udp(), m_compress_threshold(0),
                           m_min_compress_savings(0.2) {
  memcached_create(&m_memcache);
  memcached_create(&m_memcache_udp);

  if (MEMCACHEG(hash_strategy) == "consistent") {
    // need to hook up a global variable to set this
    memcached_behavior_set(&m_memcache, MEMCACHED_BEHAVIOR_DISTRIBUTION,
                           MEMCACHED_DISTRIBUTION_CONSISTENT_KETAMA);
    memcached_behavior_set(&m_memcache_udp, MEMCACHED_BEHAVIOR_DISTRIBUTION,
                           MEMCACHED_DISTRIBUTION_CONSISTENT_KETAMA);
  } else {
    memcached_behavior_set(&m_memcache, MEMCACHED_BEHAVIOR_DISTRIBUTION,
                           MEMCACHED_DISTRIBUTION_MODULA);
    memcached_behavior_set(&m_memcache_udp, MEMCACHED_BEHAVIOR_DISTRIBUTION,
                           MEMCACHED_DISTRIBUTION_MODULA);
  }

  if (MEMCACHEG(hash_function) == "fnv") {
    memcached_behavior_set(&m_memcache, MEMCACHED_BEHAVIOR_HASH,
                           MEMCACHED_HASH_FNV1A_32);
    memcached_behavior_set(&m_memcache_udp, MEMCACHED_BEHAVIOR_HASH,
                           MEMCACHED_HASH_FNV1A_32);
  } else {
    memcached_behavior_set(&m_memcache, MEMCACHED_BEHAVIOR_HASH,
                           MEMCACHED_HASH_CRC);
    memcached_behavior_set(&m_memcache_udp, MEMCACHED_BEHAVIOR_HASH,
                           MEMCACHED_HASH_CRC);
  }

  memcached_behavior_set(&m_memcache_udp, MEMCACHED_BEHAVIOR_USE_UDP, 1);
  memcached_behavior_set(&m_memcache_udp, MEMCACHED_BEHAVIOR_BINARY_PROTOCOL, 1);
  memcached_behavior_set(&m_memcache_udp, MEMCACHED_BEHAVIOR_CHECK_OPAQUE, 1);
  memcached_behavior_set(&m_memcache_udp, MEMCACHED_BEHAVIOR_NOREPLY, 0);
  memcached_behavior_set(&m_memcache_udp, MEMCACHED_BEHAVIOR_NO_BLOCK, 1);
  memcached_behavior_set(&m_memcache_udp, MEMCACHED_BEHAVIOR_UDP_ALWAYS_FLUSH, 1);
}

c_MemcachePool::~c_MemcachePool() {
  memcached_free(&m_memcache);
  memcached_free(&m_memcache_udp);
}

void c_MemcachePool::t___construct() {
  INSTANCE_METHOD_INJECTION_BUILTIN(Memcache, Memcache::__construct);
  return;
}

bool c_MemcachePool::t_connect(CStrRef host, int port /*= 0*/,
                           int timeout /*= 0*/,
                           int timeoutms /*= 0*/) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Memcache, Memcache::connect);
  memcached_return_t ret;

  if (!host.empty() && host[0] == '/') {
    ret = memcached_server_add_unix_socket(&m_memcache, host.c_str());
  } else {
    ret = memcached_server_add(&m_memcache, host.c_str(), port);
  }

  return (ret == MEMCACHED_SUCCESS);
}

bool c_MemcachePool::t_pconnect(CStrRef host, int port /*= 0*/,
                            int timeout /*= 0*/,
                            int timeoutms /*= 0*/) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Memcache, Memcache::pconnect);
  return t_connect(host, port, timeout, timeoutms);
}

String static memcache_prepare_for_storage(CVarRef var, int &flag) {
  if (var.isString()) {
    return var.toString();
  } else if (var.isNumeric() || var.isBoolean()) {
    return var.toString();
  } else {
    flag |= MMC_SERIALIZED;
    return f_serialize(var);
  }
}

Variant static memcache_fetch_from_storage(const char *payload,
                                           size_t payload_len,
                                           uint32_t flags) {
  Variant ret = null;

  if (flags & MMC_COMPRESSED) {
    raise_warning("Unable to handle compressed values yet");
    return null;
  }

  if (flags & MMC_SERIALIZED) {
    ret = f_unserialize(String(payload, payload_len, AttachLiteral));
    // raise_notice("unable to unserialize data");
  } else {
    ret = String(payload, payload_len, CopyString);
  }

  return ret;
}

bool c_MemcachePool::t_add(CStrRef key, CVarRef var, int flag /*= 0*/,
                       int expire /*= 0*/) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Memcache, Memcache::add);
  if (key.empty()) {
    raise_warning("Key cannot be empty");
    return false;
  }

  String serialized = memcache_prepare_for_storage(var, flag);

  memcached_return_t ret = memcached_add(&m_memcache,
                                        key.c_str(), key.length(),
                                        serialized.c_str(),
                                        serialized.length(),
                                        expire, flag);

  return (ret == MEMCACHED_SUCCESS);
}

bool c_MemcachePool::t_set(CStrRef key, CVarRef var, int flag /*= 0*/,
                       int expire /*= 0*/) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Memcache, Memcache::set);
  if (key.empty()) {
    raise_warning("Key cannot be empty");
    return false;
  }

  String serialized = memcache_prepare_for_storage(var, flag);

  memcached_return_t ret = memcached_set(&m_memcache,
                                        key.c_str(), key.length(),
                                        serialized.c_str(),
                                        serialized.length(),
                                        expire, flag);

  if (ret == MEMCACHED_SUCCESS) {
    return true;
  }

  return false;
}

bool c_MemcachePool::t_replace(CStrRef key, CVarRef var, int flag /*= 0*/,
                           int expire /*= 0*/) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Memcache, Memcache::replace);
  if (key.empty()) {
    raise_warning("Key cannot be empty");
    return false;
  }

  String serialized = memcache_prepare_for_storage(var, flag);

  memcached_return_t ret = memcached_replace(&m_memcache,
                                             key.c_str(), key.length(),
                                             serialized.c_str(),
                                             serialized.length(),
                                             expire, flag);
  return (ret == MEMCACHED_SUCCESS);
}

Variant c_MemcachePool::t_get(CVarRef key, VRefParam flags /*= null*/, VRefParam cas /*= null*/) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Memcache, Memcache::get);
  TAINT_OBSERVER(TAINT_BIT_ALL, TAINT_BIT_NONE);

  // Use UDP if we have any server available
  memcached_st *memc = &m_memcache;
  if (memcached_server_count(&m_memcache_udp) > 0)
    memc = &m_memcache_udp;

  std::vector<const char *> real_keys;
  std::vector<size_t> key_len;
  Array keyArr;

  if (key.is(KindOfArray)) 
    keyArr = key.toArray();
  else 
    keyArr = Array(key);

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
  memcached_result_create(memc, &result);

  while ((memcached_fetch_result(memc, &result, &ret)) != NULL) {
    if (ret != MEMCACHED_SUCCESS) {
      // should probably notify about errors
      continue;
    }

    payload     = memcached_result_value(&result);
    payload_len = memcached_result_length(&result);
    cflags      = memcached_result_flags(&result);
    ccas        = memcached_result_cas(&result);
    res_key     = memcached_result_key_value(&result);
    res_key_len = memcached_result_key_length(&result);

    curkey = String(res_key, res_key_len, CopyString);

    return_val.set(curkey, memcache_fetch_from_storage(payload, payload_len, cflags));

    if (!flags.is(KindOfNull)) {
      if (key.is(KindOfArray)) {
        getCheckedArrayRetType(flags, null, Array &);
        arr_flags.set(curkey, cflags);
      }
      else {
        flags = cflags;
      }
    }
    
    if (!cas.is(KindOfNull)) {
      if (key.is(KindOfArray)) {
        getCheckedArrayRetType(cas, null, Array &);
        arr_cas.set(curkey, ccas);
      }
      else {
        cas = ccas;
      }
    }
  }

  memcached_result_free(&result);

  if (key.is(KindOfArray))
    return return_val;
  else
    return return_val[curkey];
}

bool c_MemcachePool::t_delete(CStrRef key, int expire /*= 0*/) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Memcache, Memcache::delete);
  if (key.empty()) {
    raise_warning("Key cannot be empty");
    return false;
  }

  memcached_return_t ret = memcached_delete(&m_memcache,
                                            key.c_str(), key.length(),
                                            expire);
  return (ret == MEMCACHED_SUCCESS);
}

int64 c_MemcachePool::t_increment(CStrRef key, int offset /*= 1*/) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Memcache, Memcache::increment);
  if (key.empty()) {
    raise_warning("Key cannot be empty");
    return false;
  }

  uint64_t value;
  memcached_return_t ret = memcached_increment(&m_memcache, key.c_str(),
                                              key.length(), offset, &value);

  if (ret == MEMCACHED_SUCCESS) {
    return (int64)value;
  }

  return false;
}

int64 c_MemcachePool::t_decrement(CStrRef key, int offset /*= 1*/) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Memcache, Memcache::decrement);
  if (key.empty()) {
    raise_warning("Key cannot be empty");
    return false;
  }

  uint64_t value;
  memcached_return_t ret = memcached_decrement(&m_memcache, key.c_str(),
                                              key.length(), offset, &value);

  if (ret == MEMCACHED_SUCCESS) {
    return (int64)value;
  }

  return false;
}

bool c_MemcachePool::t_close() {
  INSTANCE_METHOD_INJECTION_BUILTIN(Memcache, Memcache::close);
  memcached_quit(&m_memcache);
  return true;
}

Variant c_MemcachePool::t_getversion() {
  INSTANCE_METHOD_INJECTION_BUILTIN(Memcache, Memcache::getversion);
  int server_count = memcached_server_count(&m_memcache);
  char version[16];
  int version_len = 0;

  if (memcached_version(&m_memcache) != MEMCACHED_SUCCESS) {
    return false;
  }

  for (int x = 0; x < server_count; x++) {
    memcached_server_instance_st instance =
      memcached_server_instance_by_position(&m_memcache, x);

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
  INSTANCE_METHOD_INJECTION_BUILTIN(Memcache, Memcache::flush);
  return memcached_flush(&m_memcache, expire) == MEMCACHED_SUCCESS;
}

bool c_MemcachePool::t_setoptimeout(int64 timeoutms) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Memcache, Memcache::setoptimeout);
  if (timeoutms < 1) {
    timeoutms = 1000; // make default
  }

  /* intentionally doing nothing for now */

  return true;
}

int c_MemcachePool::t_getserverstatus(CStrRef host, int port /* = 0 */) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Memcache, Memcache::getserverstatus);
  /* intentionally doing nothing for now */
  return 1;
}

bool c_MemcachePool::t_setcompressthreshold(int threshold,
                                        double min_savings /* = 0.2 */) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Memcache, Memcache::setcompressthreshold);
  if (threshold < 0) {
    raise_warning("threshold must be a positive integer");
    return false;
  }

  if (min_savings < 0 || min_savings > 1) {
    raise_warning("min_savings must be a float in the 0..1 range");
    return false;
  }

  m_compress_threshold = threshold;
  m_min_compress_savings = min_savings;

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
  INSTANCE_METHOD_INJECTION_BUILTIN(Memcache, Memcache::getstats);
  if (!memcached_server_count(&m_memcache)) {
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
    memcached_server_instance_by_position(&m_memcache, 0);

  memcached_stat_st stats;
  if (memcached_stat_servername(&stats, extra_args, instance->hostname,
                                instance->port) != MEMCACHED_SUCCESS) {
    return NULL;
  }

  memcached_return_t ret;
  return memcache_build_stats(&m_memcache, &stats, &ret);
}

Array c_MemcachePool::t_getextendedstats(CStrRef type /* = null_string */,
                                     int slabid /* = 0 */,
                                     int limit /* = 100 */) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Memcache, Memcache::getextendedstats);
  memcached_return_t ret;
  memcached_stat_st *stats;

  stats = memcached_stat(&m_memcache, NULL, &ret);
  if (ret != MEMCACHED_SUCCESS) {
    return NULL;
  }

  int server_count = memcached_server_count(&m_memcache);

  Array return_val;

  for (int server_id = 0; server_id < server_count; server_id++) {
    memcached_server_instance_st server;
    memcached_stat_st *stat;
    char stats_key[30] = {0};
    size_t key_len;

    server = memcached_server_instance_by_position(&m_memcache, server_id);
    stat = stats + server_id;

    Array server_stats = memcache_build_stats(&m_memcache, stat, &ret);
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
                                   bool status /* = true */,
                                   CVarRef failure_callback /* = null_variant */) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Memcache, Memcache::setserverparams);
  /* intentionally doing nothing for now */
  return true;
}

bool c_MemcachePool::t_setfailurecallback(CVarRef failure_callback) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Memcache, Memcache::setserverparams);
  /* intentionally doing nothing for now */
  return true;
}

bool c_MemcachePool::t_addserver(CStrRef host, int tcp_port, int udp_port, 
                             bool persistent, int weight, int timeout, 
                             int retry_interval, bool status) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Memcache, Memcache::addserver);
  memcached_return_t ret;

  if (!host.empty() && host[0] == '/') {
    ret = memcached_server_add_unix_socket_with_weight(&m_memcache,
                                                       host.c_str(), weight);
  } else {
    if (tcp_port > 0)
      ret = memcached_server_add_with_weight(&m_memcache, host.c_str(), 
                                             tcp_port, weight);
    else
      ret = memcached_server_add_udp_with_weight(&m_memcache_udp, host.c_str(), 
                                                 udp_port, weight);
  }

  if (ret == MEMCACHED_SUCCESS) {
    return true;
  }

  return false;
}

Variant c_MemcachePool::t___destruct() {
  INSTANCE_METHOD_INJECTION_BUILTIN(Memcache, Memcache::__destruct);
  t_close();
  return null;
}

///////////////////////////////////////////////////////////////////////////////
}
