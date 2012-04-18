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

#include <runtime/base/preg.h>
#include <runtime/base/zend/zend_url.h>
#include <runtime/base/zend/zend_multiply.h>
#include <runtime/base/zend/zend_php_config.h>
#include "php_filter.h"
#include "filter_private.h"

#if HAVE_ARPA_INET_H
# include <arpa/inet.h>
#endif

namespace HPHP {

#ifndef INADDR_NONE
# define INADDR_NONE ((unsigned long int) -1)
#endif

#define FORMAT_IPV4    4
#define FORMAT_IPV6    6
#define MAX_LENGTH_OF_LONG 20
#define SIZEOF_LONG 8

static int php_filter_parse_int(const char *str, unsigned int str_len, long *ret) { /* {{{ */
	long ctx_value;
	int sign = 0, digit = 0;
	const char *end = str + str_len;

	switch (*str) {
		case '-':
			sign = 1;
		case '+':
			str++;
		default:
			break;
	}

	/* must start with 1..9*/
	if (str < end && *str >= '1' && *str <= '9') {
		ctx_value = ((sign)?-1:1) * ((*(str++)) - '0');
	} else {
		return -1;
	}

	if ((end - str > MAX_LENGTH_OF_LONG - 1) /* number too long */
	 || (SIZEOF_LONG == 4 && (end - str == MAX_LENGTH_OF_LONG - 1) && *str > '2')) {
		/* overflow */
		return -1;
	}

	while (str < end) {
		if (*str >= '0' && *str <= '9') {
			digit = (*(str++) - '0');
			if ( (!sign) && ctx_value <= (LONG_MAX-digit)/10 ) {
				ctx_value = (ctx_value * 10) + digit;
			} else if ( sign && ctx_value >= (LONG_MIN+digit)/10) {
				ctx_value = (ctx_value * 10) - digit;
			} else {
				return -1;
			}
		} else {
			return -1;
		}
	}

	*ret = ctx_value;
	return 1;
}
/* }}} */

static int php_filter_parse_octal(const char *str, unsigned int str_len, long *ret) { /* {{{ */
	unsigned long ctx_value = 0;
	const char *end = str + str_len;

	while (str < end) {
		if (*str >= '0' && *str <= '7') {
			unsigned long n = ((*(str++)) - '0');

			if ((ctx_value > ((unsigned long)(~(long)0)) / 8) ||
				((ctx_value = ctx_value * 8) > ((unsigned long)(~(long)0)) - n)) {
				return -1;
			}
			ctx_value += n;
		} else {
			return -1;
		}
	}
	
	*ret = (long)ctx_value;
	return 1;
}
/* }}} */

static int php_filter_parse_hex(const char *str, unsigned int str_len, long *ret) { /* {{{ */
	unsigned long ctx_value = 0;
	const char *end = str + str_len;
	unsigned long n;

	while (str < end) {
		if (*str >= '0' && *str <= '9') {
			n = ((*(str++)) - '0');
		} else if (*str >= 'a' && *str <= 'f') {
			n = ((*(str++)) - ('a' - 10));
		} else if (*str >= 'A' && *str <= 'F') {
			n = ((*(str++)) - ('A' - 10));
		} else {
			return -1;
		}
		if ((ctx_value > ((unsigned long)(~(long)0)) / 16) ||
			((ctx_value = ctx_value * 16) > ((unsigned long)(~(long)0)) - n)) {
			return -1;
		}
		ctx_value += n;
	}

	*ret = (long)ctx_value;
	return 1;
}
/* }}} */

Variant php_filter_int(PHP_INPUT_FILTER_PARAM_DECL) /* {{{ */
{
	long   min_range = 0, max_range = 0;
	int    min_range_set = 0, max_range_set = 0;
	int    allow_octal = 0, allow_hex = 0;
	int	   len, error = 0;
	long   ctx_value;
	const char *p;

	/* Parse options */
    if (option_array.isArray()) {
		Array arr_options = option_array.toArray();
		if (arr_options.exists("min_range")) {
			min_range_set = 1;
			min_range = arr_options["min_range"].toInt64();
		}
		if (arr_options.exists("max_range")) {
			max_range_set = 1;
			max_range = arr_options["max_range"].toInt64();
		}
	}

	if (in_str.empty()) {
		RETURN_VALIDATION_FAILED
	}

	allow_octal = flags & FILTER_FLAG_ALLOW_OCTAL;
	allow_hex = flags & FILTER_FLAG_ALLOW_HEX;

	/* Start the validating loop */
	p = in_str.data();
	len = in_str.size();
	ctx_value = 0;

	PHP_FILTER_TRIM_DEFAULT(p, len);

	if (*p == '0') {
		p++; len--;
		if (allow_hex && (*p == 'x' || *p == 'X')) {
			p++; len--;
			if (php_filter_parse_hex(p, len, &ctx_value) < 0) {
				error = 1;
			}
		} else if (allow_octal) {
			if (php_filter_parse_octal(p, len, &ctx_value) < 0) {
				error = 1;
			}
		} else if (len != 0) {
			error = 1;
		}
	} else {
		if (php_filter_parse_int(p, len, &ctx_value) < 0) {
			error = 1;
		}
	}

	if (error > 0 || (min_range_set && (ctx_value < min_range)) || (max_range_set && (ctx_value > max_range))) {
		RETURN_VALIDATION_FAILED
	} else {
		return ctx_value;
	}
}
/* }}} */

Variant php_filter_boolean(PHP_INPUT_FILTER_PARAM_DECL) /* {{{ */
{
	const char *str = in_str.data();
	int len = in_str.size();
	int ret;

	PHP_FILTER_TRIM_DEFAULT(str, len);

	/* returns true for "1", "true", "on" and "yes"
	 * returns false for "0", "false", "off", "no", and ""
	 * null otherwise. */
	switch (len) {
		case 1:
			if (*str == '1') {
				ret = 1;
			} else if (*str == '0') {
				ret = 0;
			} else {
				ret = -1;
			}
			break;
		case 2:
			if (strncasecmp(str, "on", 2) == 0) {
				ret = 1;
			} else if (strncasecmp(str, "no", 2) == 0) {
				ret = 0;
			} else {
				ret = -1;
			}
			break;
		case 3:
			if (strncasecmp(str, "yes", 3) == 0) {
				ret = 1;
			} else if (strncasecmp(str, "off", 3) == 0) {
				ret = 0;
			} else {
				ret = -1;
			}
			break;
		case 4:
			if (strncasecmp(str, "true", 4) == 0) {
				ret = 1;
			} else {
				ret = -1;
			}
			break;
		case 5:
			if (strncasecmp(str, "false", 5) == 0) {
				ret = 0;
			} else {
				ret = -1;
			}
			break;
		default:
			ret = -1;
	}

	if (ret == -1) {	
		RETURN_VALIDATION_FAILED
	} else {
		return (bool) ret;
	}
}
/* }}} */

Variant php_filter_float(PHP_INPUT_FILTER_PARAM_DECL) /* {{{ */
{
	int len;
	const char *str, *end;
	char *num, *p;

	char dec_sep = '.';
	char tsd_sep[4] = "',.";

	int64 lval;
	double dval;

	int first, n;
	Variant ret;

	len = in_str.size();
	str = in_str.data();

	PHP_FILTER_TRIM_DEFAULT(str, len);
	end = str + len;

	if (option_array.isArray()) {
		Array arr_options = option_array.toArray();
		if (arr_options.exists("decimal")) {
			String decimal = arr_options["decimal"].toString();
			
			if (decimal.size() != 1) {
				raise_warning("decimal separator must be one char");
				RETURN_VALIDATION_FAILED
			}
			dec_sep = decimal[0];
		}
	}

	num = p = (char *) malloc(len+1);
	if (str < end && (*str == '+' || *str == '-')) {
		*p++ = *str++;
	}
	first = 1;
	while (1) {
		n = 0;
		while (str < end && *str >= '0' && *str <= '9') {
			++n;
			*p++ = *str++;
		}
		if (str == end || *str == dec_sep || *str == 'e' || *str == 'E') {
			if (!first && n != 3) {
				goto error;
			}
			if (*str == dec_sep) {
				*p++ = '.';
				str++;
				while (str < end && *str >= '0' && *str <= '9') {
					*p++ = *str++;
				}
			}
			if (*str == 'e' || *str == 'E') {
				*p++ = *str++;
				if (str < end && (*str == '+' || *str == '-')) {
					*p++ = *str++;
				}
				while (str < end && *str >= '0' && *str <= '9') {
					*p++ = *str++;
				}
			}
			break;
		}
		if ((flags & FILTER_FLAG_ALLOW_THOUSAND) && (*str == tsd_sep[0] || *str == tsd_sep[1] || *str == tsd_sep[2])) {
			if (first?(n < 1 || n > 3):(n != 3)) {
				goto error;
			}
			first = 0;
			str++;
		} else {
			goto error;
		}
	}
	if (str != end) {
		goto error;
	}
	*p = 0;

	switch (is_numeric_string(num, p - num, &lval, &dval, 0)) {
		case KindOfInt64:
			ret = lval;
			break;
		case KindOfDouble:
			if ((!dval && p - num > 1 && strpbrk(num, "123456789")) || !zend_finite(dval)) {
				free(num);
				RETURN_VALIDATION_FAILED
			} else {
				ret = dval;
			}
			break;
		default:
error:
			free(num);
			RETURN_VALIDATION_FAILED
	}
	free(num);
	return ret;
}
/* }}} */

Variant php_filter_validate_regexp(PHP_INPUT_FILTER_PARAM_DECL) /* {{{ */
{
	String regexp;
	int matches;

	/* Parse options */
	if (option_array.isArray()) {
		Array arr_options = option_array.toArray();

		if (arr_options.exists("regexp")) {
			regexp = arr_options["regexp"].toString();

		} else {
			raise_warning("'regexp' option missing");
			RETURN_VALIDATION_FAILED
		}
	}

	matches = preg_match(regexp, in_str);

	/* 0 means that the vector is too small to hold all the captured substring offsets */
	if (matches < 0) {
		RETURN_VALIDATION_FAILED
	}
	
	return in_str;
}
/* }}} */

Variant php_filter_validate_url(PHP_INPUT_FILTER_PARAM_DECL) /* {{{ */
{
	Url url;
	Variant ret;
	int old_len = in_str.size();

	ret = php_filter_url(in_str, flags, option_array, charset);


	if (!ret.isString() || old_len != ret.toString().size()) {
		RETURN_VALIDATION_FAILED
	}

	if (!url_parse(url, ret.toString().data(), ret.toString().size())) {
		RETURN_VALIDATION_FAILED
	}

	if (url.scheme != NULL && (!strcasecmp(url.scheme, "http") || !strcasecmp(url.scheme, "https"))) {
		char *e, *s;

		if (url.host == NULL) {
			goto bad_url;
		}

		e = url.host + strlen(url.host);
		s = url.host;

		/* First char of hostname must be alphanumeric */
		if(!isalnum((int)*(unsigned char *)s)) { 
			goto bad_url;
		}

		while (s < e) {
			if (!isalnum((int)*(unsigned char *)s) && *s != '-' && *s != '.') {
				goto bad_url;
			}
			s++;
		}

		if (*(e - 1) == '.') {
			goto bad_url;
		}
	}

	if (url.scheme == NULL || 
		/* some schemas allow the host to be empty */
		(url.host == NULL && (strcmp(url.scheme, "mailto") && strcmp(url.scheme, "news") && strcmp(url.scheme, "file")))
		|| ((flags & FILTER_FLAG_PATH_REQUIRED) && url.path == NULL) 
		|| ((flags & FILTER_FLAG_QUERY_REQUIRED) && url.query == NULL)
	) {
bad_url:
		RETURN_VALIDATION_FAILED
	}
	
	return in_str;
}
/* }}} */

Variant php_filter_validate_email(PHP_INPUT_FILTER_PARAM_DECL) /* {{{ */
{
	/*
	 * The regex below is based on a regex by Michael Rushton.
	 * However, it is not identical.  I changed it to only consider routeable
	 * addresses as valid.  Michael's regex considers a@b a valid address
	 * which conflicts with section 2.3.5 of RFC 5321 which states that:
	 *
	 *   Only resolvable, fully-qualified domain names (FQDNs) are permitted
	 *   when domain names are used in SMTP.  In other words, names that can
	 *   be resolved to MX RRs or address (i.e., A or AAAA) RRs (as discussed
	 *   in Section 5) are permitted, as are CNAME RRs whose targets can be
	 *   resolved, in turn, to MX or address RRs.  Local nicknames or
	 *   unqualified names MUST NOT be used.
	 *
	 * This regex does not handle comments and folding whitespace.  While
	 * this is technically valid in an email address, these parts aren't
	 * actually part of the address itself.
	 *
	 * Michael's regex carries this copyright:
	 *
	 * Copyright © Michael Rushton 2009-10
	 * http://squiloople.com/
	 * Feel free to use and redistribute this code. But please keep this copyright notice.
	 *
	 */
	String regexp("/^(?!(?:(?:\\x22?\\x5C[\\x00-\\x7E]\\x22?)|(?:\\x22?[^\\x5C\\x22]\\x22?)){255,})(?!(?:(?:\\x22?\\x5C[\\x00-\\x7E]\\x22?)|(?:\\x22?[^\\x5C\\x22]\\x22?)){65,}@)(?:(?:[\\x21\\x23-\\x27\\x2A\\x2B\\x2D\\x2F-\\x39\\x3D\\x3F\\x5E-\\x7E]+)|(?:\\x22(?:[\\x01-\\x08\\x0B\\x0C\\x0E-\\x1F\\x21\\x23-\\x5B\\x5D-\\x7F]|(?:\\x5C[\\x00-\\x7F]))*\\x22))(?:\\.(?:(?:[\\x21\\x23-\\x27\\x2A\\x2B\\x2D\\x2F-\\x39\\x3D\\x3F\\x5E-\\x7E]+)|(?:\\x22(?:[\\x01-\\x08\\x0B\\x0C\\x0E-\\x1F\\x21\\x23-\\x5B\\x5D-\\x7F]|(?:\\x5C[\\x00-\\x7F]))*\\x22)))*@(?:(?:(?!.*[^.]{64,})(?:(?:(?:xn--)?[a-z0-9]+(?:-[a-z0-9]+)*\\.){1,126}){1,}(?:(?:[a-z][a-z0-9]*)|(?:(?:xn--)[a-z0-9]+))(?:-[a-z0-9]+)*)|(?:\\[(?:(?:IPv6:(?:(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){7})|(?:(?!(?:.*[a-f0-9][:\\]]){7,})(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){0,5})?::(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){0,5})?)))|(?:(?:IPv6:(?:(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){5}:)|(?:(?!(?:.*[a-f0-9]:){5,})(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){0,3})?::(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){0,3}:)?)))?(?:(?:25[0-5])|(?:2[0-4][0-9])|(?:1[0-9]{2})|(?:[1-9]?[0-9]))(?:\\.(?:(?:25[0-5])|(?:2[0-4][0-9])|(?:1[0-9]{2})|(?:[1-9]?[0-9]))){3}))\\]))$/iD");

	int         matches;

	/* The maximum length of an e-mail address is 320 octets, per RFC 2821. */
	if (in_str.size() > 320) {
		RETURN_VALIDATION_FAILED
	}

	matches = preg_match(regexp, in_str);

	/* 0 means that the vector is too small to hold all the captured substring offsets */
	if (matches < 0) {
		RETURN_VALIDATION_FAILED
	}

	return in_str;
}
/* }}} */

static int _php_filter_validate_ipv4(const char *str, int str_len, int *ip) /* {{{ */
{
	const char *end = str + str_len;
	int num, m;
	int n = 0;

	while (str < end) {
		int leading_zero;
		if (*str < '0' || *str > '9') {
			return 0;
		}
		leading_zero = (*str == '0');
		m = 1;
		num = ((*(str++)) - '0');
		while (str < end && (*str >= '0' && *str <= '9')) {
			num = num * 10 + ((*(str++)) - '0');
			if (num > 255 || ++m > 3) {
				return 0;
			}
		}
		/* don't allow a leading 0; that introduces octal numbers,
		 * which we don't support */
		if (leading_zero && (num != 0 || m > 1))
			return 0;
		ip[n++] = num;
		if (n == 4) {
			return str == end;
		} else if (str >= end || *(str++) != '.') {
			return 0;
		}
	}
	return 0;		
}
/* }}} */

static int _php_filter_validate_ipv6(const char *str, int str_len) /* {{{ */
{
	int compressed = 0;
	int blocks = 0;
	int n;
	char *ipv4;
	const char *end;
	int ip4elm[4];
	const char *s = str;

	if (!memchr(str, ':', str_len)) {
		return 0;
	}

	/* check for bundled IPv4 */
	ipv4 = (char *)memchr(str, '.', str_len);
	if (ipv4) {
 		while (ipv4 > str && *(ipv4-1) != ':') {
			ipv4--;
		}

		if (!_php_filter_validate_ipv4(ipv4, (str_len - (ipv4 - str)), ip4elm)) {
			return 0;
		}

		str_len = ipv4 - str; /* length excluding ipv4 */
		if (str_len < 2) {
			return 0;
		}

		if (ipv4[-2] != ':') {
			/* don't include : before ipv4 unless it's a :: */
			str_len--;
		}

		blocks = 2;
	}

	end = str + str_len;

	while (str < end) {
		if (*str == ':') {
			if (++str >= end) {
				/* cannot end in : without previous : */
				return 0;
			}
			if (*str == ':') {
				if (compressed) {
					return 0;
				}
				blocks++; /* :: means 1 or more 16-bit 0 blocks */
				compressed = 1;

				if (++str == end) {
					return (blocks <= 8);
				}
			} else if ((str - 1) == s) {
				/* dont allow leading : without another : following */
				return 0;
			}				
		}
		n = 0;
		while ((str < end) &&
		       ((*str >= '0' && *str <= '9') ||
		        (*str >= 'a' && *str <= 'f') ||
		        (*str >= 'A' && *str <= 'F'))) {
			n++;
			str++;
		}
		if (n < 1 || n > 4) {
			return 0;
		}
		if (++blocks > 8)
			return 0;
	}
	return ((compressed && blocks <= 8) || blocks == 8);
}
/* }}} */

Variant php_filter_validate_ip(PHP_INPUT_FILTER_PARAM_DECL) /* {{{ */
{
	/* validates an ipv4 or ipv6 IP, based on the flag (4, 6, or both) add a
	 * flag to throw out reserved ranges; multicast ranges... etc. If both
	 * allow_ipv4 and allow_ipv6 flags flag are used, then the first dot or
	 * colon determine the format */

	int            ip[4];
	int            mode;

	if (memchr(in_str.data(), ':', in_str.size())) {
		mode = FORMAT_IPV6;
	} else if (memchr(in_str.data(), '.', in_str.size())) {
		mode = FORMAT_IPV4;
	} else {
		RETURN_VALIDATION_FAILED
	}

	if ((flags & FILTER_FLAG_IPV4) && (flags & FILTER_FLAG_IPV6)) {
		/* Both formats are cool */
	} else if ((flags & FILTER_FLAG_IPV4) && mode == FORMAT_IPV6) {
		RETURN_VALIDATION_FAILED
	} else if ((flags & FILTER_FLAG_IPV6) && mode == FORMAT_IPV4) {
		RETURN_VALIDATION_FAILED
	}

	switch (mode) {
		case FORMAT_IPV4:
			if (!_php_filter_validate_ipv4(in_str.data(), in_str.size(), ip)) {
				RETURN_VALIDATION_FAILED
			}

			/* Check flags */
			if (flags & FILTER_FLAG_NO_PRIV_RANGE) {
				if (
					(ip[0] == 10) ||
					(ip[0] == 172 && (ip[1] >= 16 && ip[1] <= 31)) ||
					(ip[0] == 192 && ip[1] == 168)
				) {
					RETURN_VALIDATION_FAILED
				}
			}

			if (flags & FILTER_FLAG_NO_RES_RANGE) {
				if (
					(ip[0] == 0) ||
					(ip[0] == 128 && ip[1] == 0) ||
					(ip[0] == 191 && ip[1] == 255) ||
					(ip[0] == 169 && ip[1] == 254) ||
					(ip[0] == 192 && ip[1] == 0 && ip[2] == 2) ||
					(ip[0] == 127 && ip[1] == 0 && ip[2] == 0 && ip[3] == 1) ||
					(ip[0] >= 224 && ip[0] <= 255)
				) {
					RETURN_VALIDATION_FAILED
				}
			}
			break;

		case FORMAT_IPV6:
			{
				int res = 0;
				res = _php_filter_validate_ipv6(in_str.data(), in_str.size());
				if (res < 1) {
					RETURN_VALIDATION_FAILED
				}
				/* Check flags */
				if (flags & FILTER_FLAG_NO_PRIV_RANGE) {
					if (in_str.size() >=2 && (!strncasecmp("FC", in_str.data(), 2) || !strncasecmp("FD", in_str.data(), 2))) {
						RETURN_VALIDATION_FAILED
					}
				}
				if (flags & FILTER_FLAG_NO_RES_RANGE) {
					switch (in_str.size()) {
						case 1: case 0:
							break;
						case 2:
							if (!strcmp("::", in_str.data())) {
								RETURN_VALIDATION_FAILED
							}
							break;
						case 3:
							if (!strcmp("::1", in_str.data()) || !strcmp("5f:", in_str.data())) {
								RETURN_VALIDATION_FAILED
							}
							break;
						default:
							if (in_str.size() >= 5) {
								if (
									!strncasecmp("fe8", in_str.data(), 3) ||
									!strncasecmp("fe9", in_str.data(), 3) ||
									!strncasecmp("fea", in_str.data(), 3) ||
									!strncasecmp("feb", in_str.data(), 3)
								) {
									RETURN_VALIDATION_FAILED
								}
							}
							if (
								(in_str.size() >= 9 &&  !strncasecmp("2001:0db8", in_str.data(), 9)) ||
								(in_str.size() >= 2 &&  !strncasecmp("5f", in_str.data(), 2)) ||
								(in_str.size() >= 4 &&  !strncasecmp("3ff3", in_str.data(), 4)) ||
								(in_str.size() >= 8 &&  !strncasecmp("2001:001", in_str.data(), 8))
							) {
								RETURN_VALIDATION_FAILED
							}
					}
				}
			}
			break;
	}

	return in_str;
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

