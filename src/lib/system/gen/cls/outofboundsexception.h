/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010 Facebook, Inc. (http://www.facebook.com)          |
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

#ifndef __GENERATED_cls_outofboundsexception_h__
#define __GENERATED_cls_outofboundsexception_h__

#include <cls/runtimeexception.h>

namespace HPHP {
///////////////////////////////////////////////////////////////////////////////

/* SRC: classes/exception.php line 75 */
class c_outofboundsexception : virtual public c_runtimeexception {
  BEGIN_CLASS_MAP(outofboundsexception)
    PARENT_CLASS(exception)
    PARENT_CLASS(runtimeexception)
  END_CLASS_MAP(outofboundsexception)
  DECLARE_CLASS(outofboundsexception, OutOfBoundsException, runtimeexception)
  DECLARE_INVOKES_FROM_EVAL
  void init();
};

///////////////////////////////////////////////////////////////////////////////
}

#endif // __GENERATED_cls_outofboundsexception_h__
