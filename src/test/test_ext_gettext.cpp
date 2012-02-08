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

#include <test/test_ext_gettext.h>
#include <runtime/ext/ext_gettext.h>

IMPLEMENT_SEP_EXTENSION_TEST(Gettext);
///////////////////////////////////////////////////////////////////////////////

bool TestExtGettext::RunTests(const std::string &which) {
  bool ret = true;

  RUN_TEST(test_textdomain);
  RUN_TEST(test_gettext);
  RUN_TEST(test_dgettext);
  RUN_TEST(test_dcgettext);
  RUN_TEST(test_bindtextdomain);
  RUN_TEST(test_ngettext);
  RUN_TEST(test_dngettext);
  RUN_TEST(test_dcngettext);
  RUN_TEST(test_bind_textdomain_codeset);

  return ret;
}

///////////////////////////////////////////////////////////////////////////////

bool TestExtGettext::test_textdomain() {
  return Count(true);
}

bool TestExtGettext::test_gettext() {
  return Count(true);
}

bool TestExtGettext::test_dgettext() {
  return Count(true);
}

bool TestExtGettext::test_dcgettext() {
  return Count(true);
}

bool TestExtGettext::test_bindtextdomain() {
  return Count(true);
}

bool TestExtGettext::test_ngettext() {
  return Count(true);
}

bool TestExtGettext::test_dngettext() {
  return Count(true);
}

bool TestExtGettext::test_dcngettext() {
  return Count(true);
}

bool TestExtGettext::test_bind_textdomain_codeset() {
  return Count(true);
}
