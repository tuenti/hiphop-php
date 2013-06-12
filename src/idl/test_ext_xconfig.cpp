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

#include <test/test_ext_xconfig.h>
#include <runtime/ext/ext_xconfig.h>

IMPLEMENT_SEP_EXTENSION_TEST(Xconfig);
///////////////////////////////////////////////////////////////////////////////

bool TestExtXconfig::RunTests(const std::string &which) {
  bool ret = true;

  RUN_TEST(test_XConfig);
  RUN_TEST(test_XConfigNode);
  RUN_TEST(test_XConfigException);
  RUN_TEST(test_XConfigNotFoundException);
  RUN_TEST(test_XConfigNotConnectedException);
  RUN_TEST(test_XConfigWrongTypeException);

  return ret;
}

///////////////////////////////////////////////////////////////////////////////

bool TestExtXconfig::test_XConfig() {
  return Count(true);
}

bool TestExtXconfig::test_XConfigNode() {
  return Count(true);
}

bool TestExtXconfig::test_XConfigException() {
  return Count(true);
}

bool TestExtXconfig::test_XConfigNotFoundException() {
  return Count(true);
}

bool TestExtXconfig::test_XConfigNotConnectedException() {
  return Count(true);
}

bool TestExtXconfig::test_XConfigWrongTypeException() {
  return Count(true);
}
