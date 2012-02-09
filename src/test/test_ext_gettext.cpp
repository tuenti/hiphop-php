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

/*
 * This tests have been basically been copy-pasted and adapted from the test of
 * the PHP gettext extension
 *
 * You will need to have installed fi_FI locale, this is needed to test
 * translations with at least two languages.
 *
 */

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
    setlocale(LC_ALL, "en_US.UTF-8");
    f_bindtextdomain ("messages", "./locale");
    VS("test", f_textdomain("test"));
    VS("test", f_textdomain(""));
    VS("foo", f_textdomain("foo"));

    return Count(true);
}

bool TestExtGettext::test_gettext() {
    bindtextdomain ("messages", "./locale");
    textdomain ("messages");

    setlocale(LC_ALL, "en_US.UTF-8");
    VS("A basic test", f_gettext("Basic test"));
    VS("A basic test", f__("Basic test"));

    if (!setlocale(LC_ALL, "fi_FI")) {
        raise_warning("skip fi_FI locale not supported.");
    }
                
    setlocale(LC_ALL, "fi_FI");
    VS("Perustesti", f__("Basic test"));
    VS("Perustesti", f_gettext("Basic test"));


    return Count(true);
}

bool TestExtGettext::test_dgettext() {
    setlocale(LC_MESSAGES, "en_US.UTF-8");
    setlocale(LC_ALL, "en_US.UTF-8");
    f_bindtextdomain("dgettextTest", "./locale");
    f_bindtextdomain("dgettextTest_switch", "./locale");
    f_textdomain("dgettextTest");
    
    VS("Produkt", f_gettext("item"));
    VS("Produkt_switched", f_dgettext("dgettextTest_switch", "item"));
    VS("Produkt", f_gettext("item"));
    return Count(true);
}

bool TestExtGettext::test_dcgettext() {
    return Count(true);
}

bool TestExtGettext::test_bindtextdomain() {
    setlocale(LC_ALL, "en_US.UTF-8");
    f_textdomain("messages");
    f_bindtextdomain("messages", "");
    VS("Basic test", f_gettext("Basic test"));
    f_bindtextdomain("messages", "./locale");
    VS("A basic test", f_gettext("Basic test"));
    VS(false, f_bindtextdomain("example.org", "foobar"));

    // Should output a HipHop warning
    VS(false, f_bindtextdomain("", "foobar"));

    return Count(true);
}

bool TestExtGettext::test_ngettext() {
    setlocale(LC_ALL, "en_US.UTF-8");
    f_bindtextdomain("dngettextTest", "./locale");
    f_textdomain("dngettextTest");
    VS("Produkt", f_ngettext("item", "items", 1));
    VS("Produkte", f_ngettext("item", "items", 2));

    return Count(true);
}

bool TestExtGettext::test_dngettext() {
    setlocale(LC_ALL, "en_US.UTF-8");
    f_bindtextdomain("dngettextTest", "./locale");

    VS("Produkt", f_dngettext("dngettextTest", "item", "items", 1));
    VS("Produkte", f_dngettext("dngettextTest", "item", "items", 2));

    return Count(true);
}

bool TestExtGettext::test_dcngettext() {
    VS(f_dcngettext(1,1,1,1,1), "1");
    VS(f_dcngettext("test","test","test",1,1), "test");
    VS(f_dcngettext("test","test","test",0,0), "test");
    VS(f_dcngettext("test","test","test",-1,-1), "test");
    VS(f_dcngettext("","","",1,1), "");
    VS(f_dcngettext("","","",0,0), "");

    setlocale(LC_MESSAGES, "en_US.UTF-8");
    setlocale(LC_ALL, "en_US.UTF-8");
    f_bindtextdomain("dngettextTest", "./locale");

    VS("cProdukt", f_dcgettext("dngettextTest", "item", LC_CTYPE));
    VS("Produkt", f_dcgettext("dngettextTest", "item", LC_MESSAGES));

    return Count(true);
}

bool TestExtGettext::test_bind_textdomain_codeset() {
    VS("UTF-8", f_bind_textdomain_codeset("messages", "UTF-8"));

    return Count(true);
}
