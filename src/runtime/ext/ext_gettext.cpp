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

#include <runtime/ext/ext_gettext.h>
#include <runtime/ext/ext_file.h>
#include <libintl.h>
#include <string.h>

namespace HPHP {
///////////////////////////////////////////////////////////////////////////////

#define PHP_GETTEXT_MAX_DOMAIN_LENGTH 1024
#define PHP_GETTEXT_MAX_MSGID_LENGTH 4096

#define PHP_GETTEXT_LENGTH_CHECK(check_name, check_content) \
    if (strlen(check_content) > PHP_GETTEXT_MAX_MSGID_LENGTH) { \
        raise_warning("%s passed is too long", check_name); \
        return false; \
    }

Variant f_textdomain(CStrRef domain) {
    const char *c_domain = domain.c_str();

    PHP_GETTEXT_LENGTH_CHECK("domain", c_domain)

    if (strcmp(c_domain, "") && strcmp(c_domain, "0")) {
        c_domain = domain.c_str();
    } else {
        c_domain = NULL;
    }   

    return String(textdomain(c_domain));
}

Variant f_gettext(CStrRef msgid) {
    const char *c_msgid = msgid.c_str();

    PHP_GETTEXT_LENGTH_CHECK("msgid", c_msgid)

    return String(gettext(c_msgid));
}

Variant f__(CStrRef msgid) {
    return f_gettext(msgid);
}

Variant f_dgettext(CStrRef domain_name, CStrRef msgid) {
    const char *c_domain = domain_name.c_str();
    const char *c_msgid = msgid.c_str();

    PHP_GETTEXT_LENGTH_CHECK("domain", c_domain)
    PHP_GETTEXT_LENGTH_CHECK("msgid", c_msgid)

    return String(dgettext(c_domain, c_msgid));
}

Variant f_dcgettext(CStrRef domain_name, CStrRef msgid, int64 category) {
    const char *c_domain = domain_name.c_str();
    const char *c_msgid = msgid.c_str();

    PHP_GETTEXT_LENGTH_CHECK("domain", c_domain)
    PHP_GETTEXT_LENGTH_CHECK("msgid", c_msgid)

    return String(dcgettext(c_domain, c_msgid, category));
}

Variant f_bindtextdomain(CStrRef domain_name, CStrRef dir) {
    const char *c_domain = domain_name.c_str();
    const char *c_dir = dir.c_str();
    Variant final_path;

    PHP_GETTEXT_LENGTH_CHECK("domain", c_domain)

    if (c_domain[0] == '\0') {
        raise_warning("The first parameter of bindtextdomain must not be empty");
        return false;
    }

    if (c_dir[0] != '\0' && strcmp(c_dir, "0")) {
        final_path = f_realpath(dir);
        if (!final_path) {
            return false;
        }
    } else {
        final_path = f_getcwd();
        if (!final_path ){
            return false;
        }
    }

    return String(bindtextdomain(c_domain, final_path.toString().c_str()));
}

Variant f_ngettext(CStrRef msg, CStrRef msg_plural, int64 n) {
    const char *c_msg = msg.c_str();
    const char *c_msg_plural = msg_plural.c_str();

    PHP_GETTEXT_LENGTH_CHECK("msg", c_msg)
    PHP_GETTEXT_LENGTH_CHECK("msg_plural", c_msg_plural)

    const char *translated_msg = ngettext(c_msg, c_msg_plural, n);
    
    if (!translated_msg)
        return false;

    return String(translated_msg);
}

Variant f_dngettext(CStrRef domain, CStrRef msg, CStrRef msg_plural, int64 n) {
    const char *c_domain = domain.c_str();
    const char *c_msg = msg.c_str();
    const char *c_msg_plural = msg_plural.c_str();

    PHP_GETTEXT_LENGTH_CHECK("domain", c_domain)
    PHP_GETTEXT_LENGTH_CHECK("msg", c_msg)
    PHP_GETTEXT_LENGTH_CHECK("msg_plural", c_msg_plural)

    const char *translated_msg = dngettext(c_domain, c_msg, c_msg_plural, n);
    
    if (!translated_msg)
        return false;

    return String(translated_msg);
}

Variant f_dcngettext(CStrRef domain, CStrRef msg, CStrRef msg_plural, int64 n, int64 category) {
    const char *c_domain = domain.c_str();
    const char *c_msg = msg.c_str();
    const char *c_msg_plural = msg_plural.c_str();

    PHP_GETTEXT_LENGTH_CHECK("domain", c_domain)
    PHP_GETTEXT_LENGTH_CHECK("msg", c_msg)
    PHP_GETTEXT_LENGTH_CHECK("msg_plural", c_msg_plural)

    const char *translated_msg = dcngettext(c_domain, c_msg, c_msg_plural, n, category);
    
    if (!translated_msg)
        return false;

    return String(translated_msg);
}

Variant f_bind_textdomain_codeset(CStrRef domain, CStrRef codeset) {
    const char *c_domain = domain.c_str();
    const char *c_codeset = codeset.c_str();

    PHP_GETTEXT_LENGTH_CHECK("domain", c_domain);

    const char *curr_codeset = bind_textdomain_codeset(c_domain, c_codeset);

    if (!curr_codeset)
        return false;

    return String(curr_codeset);
    return String();
}

///////////////////////////////////////////////////////////////////////////////
}
