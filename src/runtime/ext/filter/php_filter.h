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
#ifndef PHP_FILTER_H
#define PHP_FILTER_H

#include <runtime/base/base_includes.h>

namespace HPHP {

#define PHP_INPUT_FILTER_PARAM_DECL String in_str, long flags, CVarRef option_array, char *charset

Variant php_filter_int(PHP_INPUT_FILTER_PARAM_DECL);
Variant php_filter_boolean(PHP_INPUT_FILTER_PARAM_DECL);
Variant php_filter_float(PHP_INPUT_FILTER_PARAM_DECL);
Variant php_filter_validate_regexp(PHP_INPUT_FILTER_PARAM_DECL);
Variant php_filter_validate_url(PHP_INPUT_FILTER_PARAM_DECL);
Variant php_filter_validate_email(PHP_INPUT_FILTER_PARAM_DECL);
Variant php_filter_validate_ip(PHP_INPUT_FILTER_PARAM_DECL);

Variant php_filter_string(PHP_INPUT_FILTER_PARAM_DECL);
Variant php_filter_encoded(PHP_INPUT_FILTER_PARAM_DECL);
Variant php_filter_special_chars(PHP_INPUT_FILTER_PARAM_DECL);
Variant php_filter_full_special_chars(PHP_INPUT_FILTER_PARAM_DECL);
Variant php_filter_unsafe_raw(PHP_INPUT_FILTER_PARAM_DECL);
Variant php_filter_email(PHP_INPUT_FILTER_PARAM_DECL);
Variant php_filter_url(PHP_INPUT_FILTER_PARAM_DECL);
Variant php_filter_number_int(PHP_INPUT_FILTER_PARAM_DECL);
Variant php_filter_number_float(PHP_INPUT_FILTER_PARAM_DECL);
Variant php_filter_magic_quotes(PHP_INPUT_FILTER_PARAM_DECL);

//Variant php_filter_callback(PHP_INPUT_FILTER_PARAM_DECL);

}

#endif	/* FILTER_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 */
