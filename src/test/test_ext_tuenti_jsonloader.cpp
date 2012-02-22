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

#include <test/test_ext_tuenti_jsonloader.h>
#include <runtime/ext/ext_tuenti_jsonloader.h>
#include <runtime/ext/ext_file.h>
#include <runtime/ext/ext_json.h>
#include <stdio.h>

IMPLEMENT_SEP_EXTENSION_TEST(Tuenti_jsonloader);
///////////////////////////////////////////////////////////////////////////////

bool TestExtTuenti_jsonloader::RunTests(const std::string &which) {
  bool ret = true;

  RUN_TEST(test_cached_json_decode);

  return ret;
}

///////////////////////////////////////////////////////////////////////////////
bool TestExtTuenti_jsonloader::test_cached_json_decode() {
  String test_files[] = { "databaseConfig.php.json",
                          "memcacheConfig.php.json",
                          "/tmp/",
                          "non-existing-file",
                          ""};

  VS(0, chdir("test/json_files"));

  // First time will be uncached, subsequent ones will be cached, no special
  // reason to iterate 10 times... just to maybe catch some rare bug
  for (int j = 0; j < 10; j++) {
    for (int i = 0; i < 5; i++) {
      Variant cached = f_cached_json_decode(test_files[i], true);
      Variant notcached = f_json_decode(f_file_get_contents(test_files[i]), true);
      VS(cached[0], notcached);
    }
  }

  // Wait for refresh time and check again
  sleep(2);
  for (int i = 0; i < 5; i++) {
    Variant cached = f_cached_json_decode(test_files[i], true);
    Variant notcached = f_json_decode(f_file_get_contents(test_files[i]), true);
    VS(cached[0], notcached);
  }

  // Make test_files[0] link to test_files[2] to test cache updates
  VS(0, rename(test_files[0], "temporal_database"));
  VS(0, symlink(test_files[1], test_files[0]));

  // Now test_files[1] json should be updated accordly
  Variant cached = f_cached_json_decode(test_files[0], true);
  Variant notcached = f_json_decode(f_file_get_contents(test_files[0]), true);
  VS(cached[0], notcached);

  // Put files back to original state
  VS(0, unlink(test_files[0]));
  VS(0, rename("temporal_database", test_files[0]));

  VS(0, chdir("../.."));
  return Count(true);
}
