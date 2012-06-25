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

#include <unistd.h>
#include <test/test_ext_memcachepool.h>
#include <runtime/ext/ext_memcachepool.h>
#include <runtime/ext/ext_options.h>
#include <runtime/ext/ext_json.h>
#include <test/test_memcached_info.inc>

IMPLEMENT_SEP_EXTENSION_TEST(Memcachepool);
///////////////////////////////////////////////////////////////////////////////

bool TestExtMemcachepool::RunTests(const std::string &which) {
  bool ret = true;

  //RUN_TEST(test_MemcachePool_getInstance);
  RUN_TEST(test_MemcachePool_flush);
  RUN_TEST(test_MemcachePool_get_set);
  RUN_TEST(test_MemcachePool_add);
  RUN_TEST(test_MemcachePool_replace);
  RUN_TEST(test_MemcachePool_incdec);
  RUN_TEST(test_MemcachePool_expiration);
  RUN_TEST(test_MemcachePool_cas);
  RUN_TEST(test_MemcachePool_delete);
  RUN_TEST(test_MemcachePool_types);

  return ret;
}

///////////////////////////////////////////////////////////////////////////////

#define CREATE_MEMCACHED()                                              \
  p_MemcachePool memc(p_MemcachePool(NEWOBJ(c_MemcachePool))->create());\
  memc->t_addserver(TEST_MEMCACHED_HOSTNAME, TEST_MEMCACHED_PORT);      \
  Variant memc_version = memc->t_getversion();                          \
  if (memc_version.same(false)) {                                       \
    SKIP("No memcached running");                                       \
    return Count(true);                                                 \
  }


bool TestExtMemcachepool::test_MemcachePool_getInstance() {
  Variant empty;

  c_MemcachePool::t_getstoragememcache(3, 40, ref(empty));
  VS(empty, true);

  c_MemcachePool::t_getstoragememcache(4, 40, ref(empty));
  VS(empty, true);

  c_MemcachePool::t_getstoragememcache(2, 40, ref(empty));
  VS(empty, true);

  c_MemcachePool::t_getstoragememcache(3, 40, ref(empty));
  VS(empty, false);

  c_MemcachePool::t_getstoragememcache(3, 39, ref(empty));
  VS(empty, false);

  c_MemcachePool::t_getstoragememcache(3, 41, ref(empty));
  VS(empty, true);

  c_MemcachePool::t_getstoragememcache(3, 40, ref(empty));
  VS(empty, false);

  return Count(true);
}

bool TestExtMemcachepool::test_MemcachePool_flush() {
  CREATE_MEMCACHED();
  const char *key = "flush_test";

  // We set a key and check for it
  VERIFY(memc->t_set(key, "flush"));
  VS(memc->t_get(key), "flush");

  // We flush and test tha key is not present anymore
  VERIFY(memc->t_flush());
  VERIFY(! memc->t_get(key));

  // Same with expire time
  VERIFY(memc->t_set(key, "flush_expire"));
  VS(memc->t_get(key), "flush_expire");
  VERIFY(memc->t_flush(time(NULL) + 2));
  usleep(1000 * 500); // 0.5 sec
  VS(memc->t_get(key), "flush_expire");
  usleep(1000 * 1600); // 1.6 sec
  VERIFY(! memc->t_get(key));
 
  VERIFY(memc->t_flush());

  return Count(true);
}

bool TestExtMemcachepool::test_MemcachePool_get_set() {
  CREATE_MEMCACHED();
  Array list;
  list.add("array_empty", Array::Create());
  list.add("array_small", CREATE_VECTOR1(1));
  list.add("loooooooooooooooonger_stringgggggg", "with a loooooooooooooooonnnnnnnnnger payloadddddddddddd");
  list.add("string", "just a string");
  list.add("array", CREATE_VECTOR4(1, 2, 3, "foo"));
  list.add("map", CREATE_MAP1("fbid", 101501853510151001LL));
  Object obj = f_json_decode("{\"a\":1,\"b\":2.3,\"3\":\"test\"}", true);
  list.add("object", obj.toArray());

  // Set & check for all elements individually
  for (ArrayIter iter(list); iter; ++iter) {
    VERIFY(memc->t_set(iter.first(), iter.second(), 0, 0));
    VS(memc->t_get(iter.first()), iter.second());
  }

  // Multiget
  Variant res = memc->t_get(list.keys());
  VERIFY(res.isArray());
  Array resArray = res.toArray();
  VS(resArray->size(), list.size());
  for (ArrayIter iter(resArray); iter; ++iter) {
    VS(iter.second(), list[iter.first()]);
  }

  for (ArrayIter iter(list); iter; ++iter) {
    VERIFY(memc->t_delete(iter.first()));
  }

  return Count(true);
}

bool TestExtMemcachepool::test_MemcachePool_add() {
  CREATE_MEMCACHED();
  const char *key = "add_test";

  VERIFY(memc->t_add(key, "foo"));
  VS(memc->t_get(key), "foo");
  VERIFY(memc->t_delete(key));

  VERIFY(memc->t_add(key, "foo2"));
  VS(memc->t_get(key), "foo2");
  VERIFY(! memc->t_add(key, "foo"));
  VS(memc->t_get(key), "foo2");
  VERIFY(memc->t_delete(key));
  VERIFY(! memc->t_get(key));

  VERIFY(memc->t_add(key, "foo3"));
  VS(memc->t_get(key), "foo3");
  VERIFY(memc->t_delete(key));

  return Count(true);
}

bool TestExtMemcachepool::test_MemcachePool_replace() {
  CREATE_MEMCACHED();
  const char *key = "replace_test";

  VERIFY(memc->t_add(key, "foo"));
  VS(memc->t_get(key), "foo");
  VERIFY(memc->t_replace(key, "foo2"));
  VS(memc->t_get(key), "foo2");
  VERIFY(memc->t_replace(key, "foo3"));
  VS(memc->t_get(key), "foo3");
  VERIFY(memc->t_delete(key));
  VERIFY(! memc->t_get(key));

  VERIFY(! memc->t_replace(key, "foo4"));
  VERIFY(! memc->t_get(key));

  VERIFY(memc->t_add(key, "foo5"));
  VS(memc->t_get(key), "foo5");
  VERIFY(memc->t_replace(key, "foo6"));
  VS(memc->t_get(key), "foo6");
  VERIFY(memc->t_delete(key));
  VERIFY(! memc->t_get(key));

  return Count(true);
}

bool TestExtMemcachepool::test_MemcachePool_incdec() {
  CREATE_MEMCACHED();
  const char *key = "incdec_key";

  VERIFY(! memc->t_increment(key, 1));
  VERIFY(! memc->t_decrement(key, 1));
  VERIFY(memc->t_add(key, "0"));
  VS(memc->t_get(key), "0"); // Memcachepool extension does not store type
  VERIFY(memc->t_increment(key, 1));
  VS(memc->t_get(key), "1");
  VERIFY(memc->t_increment(key, 10));
  VS(memc->t_get(key), "11");
  VERIFY(memc->t_increment(key, 2));
  VS(memc->t_get(key), "13");
  VERIFY(memc->t_decrement(key, 2));
  VS(memc->t_get(key), "11");
  VERIFY(memc->t_decrement(key, 10));
  VS(memc->t_get(key), "1 ");
  VERIFY(memc->t_delete(key));
  VERIFY(! memc->t_increment(key, 1));
  VERIFY(! memc->t_decrement(key, 1));

  return Count(true);
}

bool TestExtMemcachepool::test_MemcachePool_expiration() {
  CREATE_MEMCACHED();
  const char *key = "expiration_key";

  VERIFY(memc->t_set(key, "fooof", 0, 1));
  VS(memc->t_get(key), "fooof");
  usleep(1000 * 2000);
  VERIFY(! memc->t_get(key));

  return Count(true);
}

bool TestExtMemcachepool::test_MemcachePool_cas() {
  CREATE_MEMCACHED();

  for (ArrayIter iter(memc_version); iter; ++iter) {
    if (!f_version_compare(iter.second().toString(), "1.3.0", ">=")) {
      SKIP("Need memcached 1.3.0 for CAS");
      return Count(true);
    }
  }

  Variant cas;
  const char *key = "cas_test";

  VERIFY(memc->t_set(key, "10"));
  VS(memc->t_get(key, null, ref(cas)), "10");
  VERIFY(!cas.isNull());

  // Set with a up-to-date cas, change should take effect
  VERIFY(memc->t_cas(key, "11", 0, 0, cas.toDouble()));
  VS(memc->t_get(key, null, ref(cas)), "11");

  // Old cas, change should not work
  VERIFY(memc->t_set(key, "12"));
  VERIFY(! memc->t_cas(key, "13", 0, 0, cas.toDouble()));
  VS(memc->t_get(key, null, ref(cas)), "12");

  // Is cas updating expiration ?
  VERIFY(memc->t_cas(key, "13", 0, 1, cas.toDouble()));
  usleep(1000 * 1500);
  VERIFY(! memc->t_get(key));

  // Testing get of many cas in multigets
  Array list;
  list.add("string", "just a string");
  list.add("more_stuff", "yeah");
  list.add("array", CREATE_VECTOR4(1, 2, 3, "foo"));
  Variant arr_cas;

  for (ArrayIter iter(list); iter; ++iter) {
    VERIFY(memc->t_set(iter.first(), iter.second(), 0, 0));
  }

  // Multiget
  Variant res = memc->t_get(list.keys(), null, ref(arr_cas));
  VERIFY(res.isArray());
  Array resArray = res.toArray();
  Array casArray = arr_cas.toArray();

  VS(resArray->size(), list.size());
  VS(casArray->size(), list.size());

  for (ArrayIter iter(resArray); iter; ++iter) {
    VS(iter.second(), list[iter.first()]);
  }

  for (ArrayIter iter(casArray); iter; ++iter) {
    memc->t_get(iter.first(), null, ref(cas));
    VS(iter.second(), cas);
  }

  for (ArrayIter iter(list); iter; ++iter) {
    VERIFY(memc->t_delete(iter.first()));
  }

  return Count(true);
}

bool TestExtMemcachepool::test_MemcachePool_delete() {
  CREATE_MEMCACHED();
  const char *key = "delete_test";
  VERIFY(memc->t_set(key, "foo"));
  VERIFY(memc->t_delete(key));
  VERIFY(! memc->t_delete(key));
  VERIFY(! memc->t_get(key));

  return Count(true);
}

bool TestExtMemcachepool::test_MemcachePool_types() {
  CREATE_MEMCACHED();

  VERIFY(memc->t_set("false", false));
  Variant res = memc->t_get("false");
  VERIFY(res.isBoolean());
  VERIFY(memc->t_delete("false"));

  VERIFY(memc->t_set("integer", 123123));
  res = memc->t_get("integer");
  VERIFY(res.isInteger());
  VERIFY(memc->t_delete("integer"));

  VERIFY(memc->t_set("float", 1.12345678));
  res = memc->t_get("float");
  VERIFY(res.isDouble());
  VERIFY(memc->t_delete("float"));

  return Count(true);
}
