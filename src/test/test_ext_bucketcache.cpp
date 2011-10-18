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

#include <test/test_ext_bucketcache.h>
#include <runtime/ext/ext_bucketcache.h>

IMPLEMENT_SEP_EXTENSION_TEST(Bucketcache);
///////////////////////////////////////////////////////////////////////////////

bool TestExtBucketcache::RunTests(const std::string &which) {
  bool ret = true;

  RUN_TEST(test_get_bucket_cache_info);
  RUN_TEST(test_set_bucket_cache_info);

  return ret;
}

///////////////////////////////////////////////////////////////////////////////

bool TestExtBucketcache::test_get_bucket_cache_info() {
  return Count(true);
}

bool TestExtBucketcache::test_set_bucket_cache_info() {
  return Count(true);
}
