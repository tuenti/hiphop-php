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

#include <runtime/ext/ext_json.h>
#include <runtime/ext/JSON_parser.h>
#include <runtime/base/zend/utf8_decode.h>
#include <runtime/base/variable_serializer.h>

namespace HPHP {
IMPLEMENT_DEFAULT_EXTENSION(json);
///////////////////////////////////////////////////////////////////////////////
const int64 k_JSON_HEX_TAG       = 1<<0;
const int64 k_JSON_HEX_AMP       = 1<<1;
const int64 k_JSON_HEX_APOS      = 1<<2;
const int64 k_JSON_HEX_QUOT      = 1<<3;
const int64 k_JSON_FORCE_OBJECT  = 1<<4;
const int64 k_JSON_NUMERIC_CHECK = 1<<5;
const int64 k_JSON_UNESCAPED_SLASHES = 1<<6;
// intentionally higher so when PHP adds more options we're fine
const int64 k_JSON_FB_LOOSE      = 1<<20;

const int64 k_JSON_ERROR_NONE             = JSON_ERROR_NONE;
const int64 k_JSON_ERROR_DEPTH            = JSON_ERROR_DEPTH;
const int64 k_JSON_ERROR_STATE_MISMATCH   = JSON_ERROR_STATE_MISMATCH;
const int64 k_JSON_ERROR_CTRL_CHAR        = JSON_ERROR_CTRL_CHAR;
const int64 k_JSON_ERROR_SYNTAX           = JSON_ERROR_SYNTAX;
const int64 k_JSON_ERROR_UTF8             = JSON_ERROR_UTF8;
const int64 k_JSON_ERROR_RECURSION        = JSON_ERROR_RECURSION;
const int64 k_JSON_ERROR_INF_OR_NAN       = JSON_ERROR_INF_OR_NAN;
const int64 k_JSON_ERROR_UNSUPPORTED_TYPE = JSON_ERROR_UNSUPPORTED_TYPE;

///////////////////////////////////////////////////////////////////////////////

class JsonRequestData : public RequestEventHandler {
public:
  virtual void requestInit() {
    last_error = k_JSON_ERROR_NONE;
  }

  virtual void requestShutdown() {
    last_error = k_JSON_ERROR_NONE;
  }

  int64 last_error;
};

IMPLEMENT_STATIC_REQUEST_LOCAL(JsonRequestData, s_json_data);

String f_json_encode(CVarRef value, CVarRef options /* = 0 */) {
  int64 json_options = options.toInt64();
  if (options.isBoolean() && options.toBooleanVal()) {
    json_options = k_JSON_FB_LOOSE;
  }

  VariableSerializer vs(VariableSerializer::JSON, json_options);
  return vs.serialize(value, true);
}

Variant f_json_decode(CStrRef json, bool assoc /* = false */,
                      CVarRef options /* = 0 */) {
  if (json.empty()) {
    return null;
  }

  int64 json_options = options.toInt64();;
  if (options.isBoolean() && options.toBooleanVal()) {
    json_options = k_JSON_FB_LOOSE;
  }

  Variant z;
  s_json_data->last_error = JSON_parser(z, json.data(), json.size(), assoc, (json_options & k_JSON_FB_LOOSE));
  if (s_json_data->last_error == JSON_ERROR_NONE) {
    return z;
  }

  if (json.size() == 4) {
    if (!strcasecmp(json.data(), "null")) return null;
    if (!strcasecmp(json.data(), "true")) return true;
  } else if (json.size() == 5 && !strcasecmp(json.data(), "false")) {
    return false;
  }

  int64 p;
  double d;
  DataType type = json->isNumericWithVal(p, d, 0);
  if (type == KindOfInt64) {
    return p;
  } else if (type == KindOfDouble) {
    return d;
  }

  return null;
}

int64 f_json_last_error() {
  return s_json_data->last_error;
}

String f_json_last_error_msg() {
  switch(s_json_data->last_error) {
    case JSON_ERROR_NONE:
      return  "No error";
    case JSON_ERROR_DEPTH:
      return "Maximum stack depth exceeded";
    case JSON_ERROR_STATE_MISMATCH:
      return "State mismatch (invalid or malformed JSON)";
    case JSON_ERROR_CTRL_CHAR:
      return "Control character error, possibly incorrectly encoded";
    case JSON_ERROR_SYNTAX:
      return "Syntax error";
    case JSON_ERROR_UTF8:
      return "Malformed UTF-8 characters, possibly incorrectly encoded";
    case JSON_ERROR_RECURSION:
      return "Recursion detected";
    case JSON_ERROR_INF_OR_NAN:
      return "Inf and NaN cannot be JSON encoded";
    case JSON_ERROR_UNSUPPORTED_TYPE:
      return "Type is not supported";
    default:
      return "Unknown error";
  }
}

///////////////////////////////////////////////////////////////////////////////
}
