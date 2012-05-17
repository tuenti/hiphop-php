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

#include <runtime/ext/ext_fixes.h>
#include <runtime/ext/ext_datetime.h>

namespace HPHP {
///////////////////////////////////////////////////////////////////////////////

const int64 k_CAL_GREGORIAN = 0;

int64 f_cal_days_in_month(int64 calendar, int64 month, int64 year) {
  if (calendar != k_CAL_GREGORIAN)  
    throw NotImplementedException("Only CAL_GREGORIAN is supported");

  return f_date("t", f_mktime(0, 0, 0, month, 1, year));
}

}
