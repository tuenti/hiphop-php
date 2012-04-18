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

#include <runtime/ext/ext_filter.h>
#include <runtime/ext/filter/filter_private.h>
#include <runtime/ext/filter/php_filter.h>

namespace HPHP {

const int64 k_INPUT_POST = 0;
const int64 k_INPUT_GET = 0;
const int64 k_INPUT_COOKIE = 0;
const int64 k_INPUT_ENV = 0;
const int64 k_INPUT_SERVER = 0;
const int64 k_INPUT_SESSION = 0;
const int64 k_INPUT_REQUEST = 0;

const int64 k_FILTER_FLAG_NONE = FILTER_FLAG_NONE;
const int64 k_FILTER_REQUIRE_SCALAR = FILTER_REQUIRE_SCALAR;
const int64 k_FILTER_REQUIRE_ARRAY = FILTER_REQUIRE_ARRAY;
const int64 k_FILTER_FORCE_ARRAY = FILTER_FORCE_ARRAY;
const int64 k_FILTER_NULL_ON_FAILURE = FILTER_NULL_ON_FAILURE;
const int64 k_FILTER_VALIDATE_INT = FILTER_VALIDATE_INT;
const int64 k_FILTER_VALIDATE_BOOLEAN = FILTER_VALIDATE_BOOLEAN;
const int64 k_FILTER_VALIDATE_FLOAT = FILTER_VALIDATE_FLOAT;
const int64 k_FILTER_VALIDATE_REGEXP = FILTER_VALIDATE_REGEXP;
const int64 k_FILTER_VALIDATE_URL = FILTER_VALIDATE_URL;
const int64 k_FILTER_VALIDATE_EMAIL = FILTER_VALIDATE_EMAIL;
const int64 k_FILTER_VALIDATE_IP = FILTER_VALIDATE_IP;
const int64 k_FILTER_DEFAULT = FILTER_DEFAULT;
const int64 k_FILTER_UNSAFE_RAW = FILTER_UNSAFE_RAW;
const int64 k_FILTER_SANITIZE_STRING = FILTER_SANITIZE_STRING;
const int64 k_FILTER_SANITIZE_STRIPPED = FILTER_SANITIZE_STRING;
const int64 k_FILTER_SANITIZE_ENCODED = FILTER_SANITIZE_ENCODED;
const int64 k_FILTER_SANITIZE_SPECIAL_CHARS = FILTER_SANITIZE_SPECIAL_CHARS;
const int64 k_FILTER_SANITIZE_FULL_SPECIAL_CHARS = FILTER_SANITIZE_FULL_SPECIAL_CHARS;
const int64 k_FILTER_SANITIZE_EMAIL = FILTER_SANITIZE_EMAIL;
const int64 k_FILTER_SANITIZE_URL = FILTER_SANITIZE_URL;
const int64 k_FILTER_SANITIZE_NUMBER_INT = FILTER_SANITIZE_NUMBER_INT;
const int64 k_FILTER_SANITIZE_NUMBER_FLOAT = FILTER_SANITIZE_NUMBER_FLOAT;
const int64 k_FILTER_SANITIZE_MAGIC_QUOTES = FILTER_SANITIZE_MAGIC_QUOTES;
const int64 k_FILTER_CALLBACK = FILTER_CALLBACK;
const int64 k_FILTER_FLAG_ALLOW_OCTAL = FILTER_FLAG_ALLOW_OCTAL;
const int64 k_FILTER_FLAG_ALLOW_HEX = FILTER_FLAG_ALLOW_HEX;
const int64 k_FILTER_FLAG_STRIP_LOW = FILTER_FLAG_STRIP_LOW;
const int64 k_FILTER_FLAG_STRIP_HIGH = FILTER_FLAG_STRIP_HIGH;
const int64 k_FILTER_FLAG_STRIP_BACKTICK = FILTER_FLAG_STRIP_BACKTICK;
const int64 k_FILTER_FLAG_ENCODE_LOW = FILTER_FLAG_ENCODE_LOW;
const int64 k_FILTER_FLAG_ENCODE_HIGH = FILTER_FLAG_ENCODE_HIGH;
const int64 k_FILTER_FLAG_ENCODE_AMP = FILTER_FLAG_ENCODE_AMP;
const int64 k_FILTER_FLAG_NO_ENCODE_QUOTES = FILTER_FLAG_NO_ENCODE_QUOTES;
const int64 k_FILTER_FLAG_EMPTY_STRING_NULL = FILTER_FLAG_EMPTY_STRING_NULL;
const int64 k_FILTER_FLAG_ALLOW_FRACTION = FILTER_FLAG_ALLOW_FRACTION;
const int64 k_FILTER_FLAG_ALLOW_THOUSAND = FILTER_FLAG_ALLOW_THOUSAND;
const int64 k_FILTER_FLAG_ALLOW_SCIENTIFIC = FILTER_FLAG_ALLOW_SCIENTIFIC;
const int64 k_FILTER_FLAG_SCHEME_REQUIRED = FILTER_FLAG_SCHEME_REQUIRED;
const int64 k_FILTER_FLAG_HOST_REQUIRED = FILTER_FLAG_HOST_REQUIRED;
const int64 k_FILTER_FLAG_PATH_REQUIRED = FILTER_FLAG_PATH_REQUIRED;
const int64 k_FILTER_FLAG_QUERY_REQUIRED = FILTER_FLAG_QUERY_REQUIRED;
const int64 k_FILTER_FLAG_IPV4 = FILTER_FLAG_IPV4;
const int64 k_FILTER_FLAG_IPV6 = FILTER_FLAG_IPV6;
const int64 k_FILTER_FLAG_NO_RES_RANGE = FILTER_FLAG_NO_RES_RANGE;
const int64 k_FILTER_FLAG_NO_PRIV_RANGE = FILTER_FLAG_NO_PRIV_RANGE;


typedef struct filter_list_entry {
    const char *name;
    int    id; 
    Variant (*function)(PHP_INPUT_FILTER_PARAM_DECL);
} filter_list_entry;

static const filter_list_entry filter_list[] = { 
    { "int",             FILTER_VALIDATE_INT,           php_filter_int             },  
    { "boolean",         FILTER_VALIDATE_BOOLEAN,       php_filter_boolean         },  
    { "float",           FILTER_VALIDATE_FLOAT,         php_filter_float           },  

    { "validate_regexp", FILTER_VALIDATE_REGEXP,        php_filter_validate_regexp },
    { "validate_url",    FILTER_VALIDATE_URL,           php_filter_validate_url    },  
    { "validate_email",  FILTER_VALIDATE_EMAIL,         php_filter_validate_email  },  
    { "validate_ip",     FILTER_VALIDATE_IP,            php_filter_validate_ip     },  

    { "string",          FILTER_SANITIZE_STRING,        php_filter_string          },  
    { "stripped",        FILTER_SANITIZE_STRING,        php_filter_string          },  
    { "encoded",         FILTER_SANITIZE_ENCODED,       php_filter_encoded         },  
    { "special_chars",   FILTER_SANITIZE_SPECIAL_CHARS, php_filter_special_chars   },  
    { "full_special_chars",   FILTER_SANITIZE_FULL_SPECIAL_CHARS, php_filter_full_special_chars   },  
    { "unsafe_raw",      FILTER_UNSAFE_RAW,             php_filter_unsafe_raw      },  
    { "email",           FILTER_SANITIZE_EMAIL,         php_filter_email           },  
    { "url",             FILTER_SANITIZE_URL,           php_filter_url             },  
    { "number_int",      FILTER_SANITIZE_NUMBER_INT,    php_filter_number_int      },  
    { "number_float",    FILTER_SANITIZE_NUMBER_FLOAT,  php_filter_number_float    },  
    { "magic_quotes",    FILTER_SANITIZE_MAGIC_QUOTES,  php_filter_magic_quotes    },  

    //{ "callback",        FILTER_CALLBACK,               php_filter_callback        },  
};

static Variant php_zval_filter_recursive(CVarRef variable, long filter, long flags, CVarRef options, char *charset);
static Variant php_zval_filter(CVarRef variable, long filter, long flags, CVarRef options, char* charset);

static bool php_filter_id_exists(long id) {
    return ((id >= FILTER_SANITIZE_ALL && id <= FILTER_SANITIZE_LAST)
            || (id >= FILTER_VALIDATE_ALL && id <= FILTER_VALIDATE_LAST)
            || id == FILTER_CALLBACK);
}

static Variant php_filter_call(CVarRef variable, long filter, CVarRef options, long flags)
{
	Variant filter_options;
	Variant ret;
	char  *charset = NULL;

	if (options.isInteger()) {
		long lval = options.toInt32();

		if (filter != -1) { /* handler for array apply */
			/* options is the flags */
			flags = lval;

			if (!(flags & FILTER_REQUIRE_ARRAY ||  flags & FILTER_FORCE_ARRAY)) {
				flags |= FILTER_REQUIRE_SCALAR;
			}
		} else {
			filter = lval;
		}
	} 
    else if (options.isArray()) {
        Array arr_options = options.toArray();
        
        if (arr_options.exists("filter")) {
	    	filter = arr_options["filter"].toInt64();
    	}

    	if (arr_options.exists("flags")) {
	    	flags = arr_options["flags"].toInt64();

    		if (!(flags & FILTER_REQUIRE_ARRAY ||  flags & FILTER_FORCE_ARRAY)) {
	    		flags |= FILTER_REQUIRE_SCALAR;
	    	}
    	}

    	if (arr_options.exists("options")) {
    		if (filter != FILTER_CALLBACK) {
    			if (arr_options["options"].isArray()) {
    				filter_options = arr_options["options"];
    			}
    		} else {
    			filter_options = arr_options["options"];
    			flags = 0;
    		}
    	}
    }

	if (variable.isArray()) {
		if (flags & FILTER_REQUIRE_SCALAR) {
			RETURN_VALIDATION_FAILED
		}

		return php_zval_filter_recursive(variable, filter, flags, options, charset);
	}

	ret = php_zval_filter(variable, filter, flags, options, charset);

	if (flags & FILTER_FORCE_ARRAY) {
		ret = CREATE_VECTOR1(ret);
	}

	return ret;
}

static filter_list_entry php_find_filter(long id)
{
	int i, size = sizeof(filter_list) / sizeof(filter_list_entry);

	for (i = 0; i < size; ++i) {
		if (filter_list[i].id == id) {
			return filter_list[i];
		}
	}
	/* Fallback to "string" filter */
	for (i = 0; i < size; ++i) {
		if (filter_list[i].id == FILTER_DEFAULT) {
			return filter_list[i];
		}
	}
	/* To shut up GCC */
	return filter_list[0];
}

static Variant php_zval_filter(CVarRef variable, long filter, long flags, CVarRef options, char* charset)
{
	Variant ret;

	filter_list_entry  filter_func;

	filter_func = php_find_filter(filter);

	if (!filter_func.id) {
		/* Find default filter */
		filter_func = php_find_filter(FILTER_DEFAULT);
	}

	// Not sure if needed
	/*
	if (variable.isObject()) {
		return false;
	}
	*/

	ret = filter_func.function(variable.toString(), flags, options, charset);

	if (options.toArray().exists("default") &&
		((flags & FILTER_NULL_ON_FAILURE && ret.isNull()) ||
		(!(flags & FILTER_NULL_ON_FAILURE) && ret.isBoolean() && ret.toBoolean() == false))) {
		ret = options.toArray()["default"];
	}

	return ret;
}

static Variant php_zval_filter_recursive(CVarRef variable, long filter, long flags, CVarRef options, char *charset)
{
	Variant ret;

	if (!variable.isArray()) {
		ret = php_zval_filter(variable, filter, flags, options, charset);
	}
	else {
		ret = Array::Create();
		for (ArrayIter iter(variable); iter; ++iter) {
			ret.set(iter.first(), php_zval_filter_recursive(iter.second(), filter, flags, options, charset));
		}
	}

	return ret;
}

static Variant php_filter_array_handler(CVarRef variable, CVarRef options)
{
    if (options.isInteger()) {
        return php_filter_call(variable, options.toInt64(), null, FILTER_REQUIRE_ARRAY);
    } else if (options.isArray()) {
        Variant ret = Array::Create();

        for (ArrayIter iter(variable); iter; ++iter) {
            if (!iter.first().isString()) {
                raise_warning("Numeric keys are not allowed in the definition array");
                return false;
            }
            if (iter.first().toString().empty()) {
                raise_warning("Empty keys are not allowed in the definition array");
                return false;
            }

            String key = iter.first().toString();
            Array arr_var = variable.toArray();

            if (arr_var.exists(key)) {
                ret.set(key, php_filter_call(arr_var[key], -1, iter.second(), FILTER_REQUIRE_SCALAR));
            }
        }

        return ret;
    } else {
        return false;
    }   
}

bool f_filter_has_var(int type, CStrRef variable_name) {
  throw NotImplementedException(__func__);
}

Variant f_filter_id(CStrRef filtername) {
	int size = sizeof(filter_list) / sizeof(filter_list_entry);

	for (int i = 0; i < size; ++i) {
		if (strcmp(filter_list[i].name, filtername.data()) == 0) {
			return filter_list[i].id;
		}
	}

	return false;
}

Variant f_filter_input_array(int type, CVarRef definition /* = k_FILTER_DEFAULT */) {
  throw NotImplementedException(__func__);
}

Variant f_filter_input(int type, CStrRef variable_name, int filter /* = k_FILTER_DEFAULT */, CVarRef options /* = null_array */) {
  throw NotImplementedException(__func__);
}

Variant f_filter_list() {
	int size = sizeof(filter_list) / sizeof(filter_list_entry);

    Array arr = Array::Create();
	for (int i = 0; i < size; ++i) {
		arr.set(filter_list[i].name, 1);
	}
	
	return arr;
}

Variant f_filter_var_array(CArrRef data, CVarRef definition /* = k_FILTER_DEFAULT */) {
    if (definition.isInteger() && !php_filter_id_exists(definition.toInt64())) {
        return false;
    }

    return php_filter_array_handler(data, definition);
}

Variant f_filter_var(CVarRef variable, int filter /* = k_FILTER_DEFAULT */, CVarRef options /* = null_array */) {
    if (!php_filter_id_exists(filter)) {
		return false;
	}

	return php_filter_call(variable, filter, options, FILTER_REQUIRE_SCALAR);
}

}
