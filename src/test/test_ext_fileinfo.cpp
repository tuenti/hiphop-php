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

#include <test/test_ext_fileinfo.h>
#include <runtime/ext/ext_fileinfo.h>

IMPLEMENT_SEP_EXTENSION_TEST(Fileinfo);
///////////////////////////////////////////////////////////////////////////////

bool TestExtFileinfo::RunTests(const std::string &which) {
  bool ret = true;

  RUN_TEST(test_finfo_open);
  RUN_TEST(test_finfo_close);
  RUN_TEST(test_finfo_buffer);
  RUN_TEST(test_finfo_file);
  RUN_TEST(test_finfo_set_flags);

  return ret;
}

///////////////////////////////////////////////////////////////////////////////

bool TestExtFileinfo::test_finfo_open() {
  return Count(true);
}

bool TestExtFileinfo::test_finfo_close() {
  return Count(true);
}

bool TestExtFileinfo::test_finfo_buffer() {
  return Count(true);
}

bool TestExtFileinfo::test_finfo_file() {
  return Count(true);
}

bool TestExtFileinfo::test_finfo_set_flags() {
  return Count(true);
}
