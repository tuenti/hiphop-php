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

/* $Id: sanitizing_filters.c 309920 2011-04-03 16:30:31Z iliaa $ */

#include <runtime/base/string_util.h>
#include "php_filter.h"
#include "filter_private.h"
#include "util/smart_str.h"

namespace HPHP {

/* {{{ STRUCTS */
typedef unsigned long filter_map[256];
/* }}} */

static String php_filter_encode_html(String in_str, const unsigned char *chars)
{
    if (in_str.empty()) {
        return in_str;
    }   

    smart_str str = {0};

    for (int i = 0; i <in_str.size(); i++) {
        char c = in_str[i];
        if (chars[(short)c]) {
            smart_str_appendl(&str, "&#", 2); 
            smart_str_append_unsigned(&str, (unsigned long)c);
            smart_str_appendc(&str, ';');
        } else {
            /* XXX: this needs to be optimized to work with blocks of 'safe' chars */
            smart_str_appendc(&str, c); 
        }   
    }   

	String out_str = String(str.c, str.len, CopyString);
    smart_str_free(&str);

	return out_str;
}

static const unsigned char hexchars[] = "0123456789ABCDEF";

#define LOWALPHA    "abcdefghijklmnopqrstuvwxyz"
#define HIALPHA     "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define DIGIT       "0123456789"

#define DEFAULT_URL_ENCODE    LOWALPHA HIALPHA DIGIT "-._"

static String php_filter_encode_url(String in_str, const unsigned char* chars, const int char_len, int high, int low, int encode_nul)
{
	unsigned char *str, *p;
	unsigned char tmp[256];
	unsigned char *s = (unsigned char *)chars;
	unsigned char *e = s + char_len;

	memset(tmp, 1, sizeof(tmp)-1);

	while (s < e) {
		tmp[*s++] = 0;
	}

	p = str = (unsigned char *) malloc(3 * in_str.size() + 1);
	s = (unsigned char *) in_str.data();
	e = s + in_str.size();

	while (s < e) {
		if (tmp[*s]) {
			*p++ = '%';
			*p++ = hexchars[(unsigned char) *s >> 4];
			*p++ = hexchars[(unsigned char) *s & 15];
		} else {
			*p++ = *s;	
		}
		s++;	
	}
	*p = '\0';
	return String((const char *)str, p - str, AttachString);
}

static Variant php_filter_strip(String in_str, long flags)
{
	unsigned char *buf, *str;
	int   i, c;
	
	/* Optimization for if no strip flags are set */
	if (! ((flags & FILTER_FLAG_STRIP_LOW) || (flags & FILTER_FLAG_STRIP_HIGH)) ) {
		return in_str;
	}

	str = (unsigned char *) in_str.c_str();
	buf = (unsigned char *) malloc(in_str.size() + 2);
	c = 0;
	for (i = 0; i < in_str.size(); i++) {
		if ((in_str[i] > 127) && (flags & FILTER_FLAG_STRIP_HIGH)) {
		} else if ((in_str[i] < 32) && (flags & FILTER_FLAG_STRIP_LOW)) {
		} else if ((in_str[i] == '`') && (flags & FILTER_FLAG_STRIP_BACKTICK)) {
		} else {
			buf[c] = in_str[i];
			++c;
		}
	}
	buf[c] = '\0';
	return Variant(String((const char *)buf, AttachString));
}
/* }}} */

/* {{{ FILTER MAP HELPERS */
static void filter_map_init(filter_map *map)
{
	memset(map, 0, sizeof(filter_map));
}

static void filter_map_update(filter_map *map, int flag, const unsigned char *allowed_list)
{
	int l, i;

	l = strlen((const char*)allowed_list);
	for (i = 0; i < l; ++i) {
		(*map)[allowed_list[i]] = flag;
	}
}

static Variant filter_map_apply(String in_str, filter_map *map)
{
	unsigned char *buf;
	int   i, c;
	
	buf = (unsigned char *) malloc(in_str.size() + 2);

	c = 0;
	for (i = 0; i < in_str.size(); i++) {
		if ((*map)[(short)in_str[i]]) {
			buf[c] = in_str[i];
			++c;
		}
	}
	/* update string data */
	buf[c] = '\0';
	return Variant(String((const char *) buf, AttachString));
}
/* }}} */

/* {{{ php_filter_string */
Variant php_filter_string(PHP_INPUT_FILTER_PARAM_DECL)
{
	unsigned char enc[256] = {0};

	/* strip high/strip low ( see flags )*/
	in_str = php_filter_strip(in_str, flags);

	if (!(flags & FILTER_FLAG_NO_ENCODE_QUOTES)) {
		enc['\''] = enc['"'] = 1;
	}
	if (flags & FILTER_FLAG_ENCODE_AMP) {
		enc['&'] = 1;
	}
	if (flags & FILTER_FLAG_ENCODE_LOW) {
		memset(enc, 1, 32);
	}
	if (flags & FILTER_FLAG_ENCODE_HIGH) {
		memset(enc + 127, 1, sizeof(enc) - 127);
	}

	in_str = php_filter_encode_html(in_str, enc);

	/* strip tags, implicitly also removes \0 chars */
	in_str = StringUtil::StripHTMLTags(in_str);

	if (!in_str.empty())
		return in_str;

	if (flags & FILTER_FLAG_EMPTY_STRING_NULL) {
		return null;
	} else {
		return "";
	}
}
/* }}} */

/* {{{ php_filter_encoded */
Variant php_filter_encoded(PHP_INPUT_FILTER_PARAM_DECL)
{
	/* apply strip_high and strip_low filters */
	in_str = php_filter_strip(in_str, flags);
	/* urlencode */
	return php_filter_encode_url(in_str, 
								(unsigned char *)DEFAULT_URL_ENCODE,
								sizeof(DEFAULT_URL_ENCODE)-1, 
								flags & FILTER_FLAG_ENCODE_HIGH, 
								flags & FILTER_FLAG_ENCODE_LOW, 
								1);
}
/* }}} */

/* {{{ php_filter_special_chars */
Variant php_filter_special_chars(PHP_INPUT_FILTER_PARAM_DECL)
{
	unsigned char enc[256] = {0};

	in_str = php_filter_strip(in_str, flags);

	/* encodes ' " < > & \0 to numerical entities */
	enc['\''] = enc['"'] = enc['<'] = enc['>'] = enc['&'] = enc[0] = 1;

	/* if strip low is not set, then we encode them as &#xx; */
	memset(enc, 1, 32);

	if (flags & FILTER_FLAG_ENCODE_HIGH) {
		memset(enc + 127, 1, sizeof(enc) - 127);
	}
	
	return php_filter_encode_html(in_str, enc);	
}
/* }}} */

/* {{{ php_filter_full_special_chars */
Variant php_filter_full_special_chars(PHP_INPUT_FILTER_PARAM_DECL)
{
	StringUtil::QuoteStyle quotes;
	
	if (!(flags & FILTER_FLAG_NO_ENCODE_QUOTES)) {
		quotes = StringUtil::NoQuotes;
	} else {
		quotes = StringUtil::BothQuotes;
	}

    return StringUtil::HtmlEncode(in_str, quotes, "UTF-8", true);
}
/* }}} */

/* {{{ php_filter_unsafe_raw */
Variant php_filter_unsafe_raw(PHP_INPUT_FILTER_PARAM_DECL)
{
	/* Only if no flags are set (optimization) */
	if (flags != 0 && !in_str.empty()) {
		unsigned char enc[256] = {0};

		in_str = php_filter_strip(in_str, flags);

		if (flags & FILTER_FLAG_ENCODE_AMP) {
			enc['&'] = 1;
		}
		if (flags & FILTER_FLAG_ENCODE_LOW) {
			memset(enc, 1, 32);
		}
		if (flags & FILTER_FLAG_ENCODE_HIGH) {
			memset(enc + 127, 1, sizeof(enc) - 127);
		}

		return php_filter_encode_html(in_str, enc);	
	} else if (flags & FILTER_FLAG_EMPTY_STRING_NULL && in_str.empty()) {
		return NULL;
	}

	return in_str;
}
/* }}} */



/* {{{ php_filter_email */
#define SAFE        "$-_.+"
#define EXTRA       "!*'(),"
#define NATIONAL    "{}|\\^~[]`"
#define PUNCTUATION "<>#%\""
#define RESERVED    ";/?:@&="

Variant php_filter_email(PHP_INPUT_FILTER_PARAM_DECL)
{
	/* Check section 6 of rfc 822 http://www.faqs.org/rfcs/rfc822.html */
	const unsigned char allowed_list[] = LOWALPHA HIALPHA DIGIT "!#$%&'*+-=?^_`{|}~@.[]";
	filter_map     map;

	filter_map_init(&map);
	filter_map_update(&map, 1, allowed_list);
	return filter_map_apply(in_str, &map);
}
/* }}} */

/* {{{ php_filter_url */
Variant php_filter_url(PHP_INPUT_FILTER_PARAM_DECL)
{
	/* Strip all chars not part of section 5 of
	 * http://www.faqs.org/rfcs/rfc1738.html */
	const unsigned char allowed_list[] = LOWALPHA HIALPHA DIGIT SAFE EXTRA NATIONAL PUNCTUATION RESERVED;
	filter_map     map;

	filter_map_init(&map);
	filter_map_update(&map, 1, allowed_list);
	return filter_map_apply(in_str, &map);
}
/* }}} */

/* {{{ php_filter_number_int */
Variant php_filter_number_int(PHP_INPUT_FILTER_PARAM_DECL)
{
	/* strip everything [^0-9+-] */
	const unsigned char allowed_list[] = "+-" DIGIT;
	filter_map     map;

	filter_map_init(&map);
	filter_map_update(&map, 1, allowed_list);
	return filter_map_apply(in_str, &map);
}
/* }}} */

/* {{{ php_filter_number_float */
Variant php_filter_number_float(PHP_INPUT_FILTER_PARAM_DECL)
{
	/* strip everything [^0-9+-] */
	const unsigned char allowed_list[] = "+-" DIGIT;
	filter_map     map;

	filter_map_init(&map);
	filter_map_update(&map, 1, allowed_list);

	/* depending on flags, strip '.', 'e', ",", "'" */
	if (flags & FILTER_FLAG_ALLOW_FRACTION) {
		filter_map_update(&map, 2, (const unsigned char *) ".");
	}
	if (flags & FILTER_FLAG_ALLOW_THOUSAND) {
		filter_map_update(&map, 3,  (const unsigned char *) ",");
	}
	if (flags & FILTER_FLAG_ALLOW_SCIENTIFIC) {
		filter_map_update(&map, 4,  (const unsigned char *) "eE");
	}
	return filter_map_apply(in_str, &map);
}
/* }}} */

/* {{{ php_filter_magic_quotes */
Variant php_filter_magic_quotes(PHP_INPUT_FILTER_PARAM_DECL)
{
	// SqlEncode is mostly equivalent to php_addslashes
	return StringUtil::SqlEncode(in_str);
}
/* }}} */

}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
