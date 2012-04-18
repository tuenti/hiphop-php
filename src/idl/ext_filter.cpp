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

#include <runtime/ext/ext_filter.h>

namespace HPHP {
///////////////////////////////////////////////////////////////////////////////

bool f_filter_has_var(int type, CStrRef variable_name) {
  throw NotImplementedException(__func__);
}

Variant f_filter_id(CStrRef filtername) {
  throw NotImplementedException(__func__);
}

Variant f_filter_input_array(int type, CVarRef definition /* = k_FILTER_DEFAULT */) {
  throw NotImplementedException(__func__);
}

Variant f_filter_input(int type, CStrRef variable_name, int filter /* = k_FILTER_DEFAULT */, CVarRef options /* = null_array */) {
  throw NotImplementedException(__func__);
}

Variant f_filter_list() {
  throw NotImplementedException(__func__);
}

Variant f_filter_var_array(CArrRef data, CVarRef definition /* = k_FILTER_DEFAULT */) {
  throw NotImplementedException(__func__);
}

Variant f_filter_var(CVarRef variable, int filter /* = k_FILTER_DEFAULT */, CVarRef options /* = null_array */) {
  throw NotImplementedException(__func__);
}


///////////////////////////////////////////////////////////////////////////////
}
