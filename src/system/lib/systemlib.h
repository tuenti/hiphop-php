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

#include <runtime/base/types.h>

#ifndef __SYSTEMLIB_H__
#define __SYSTEMLIB_H__

namespace HPHP {
///////////////////////////////////////////////////////////////////////////////

class SystemLib {
 public:
  static ObjectData* AllocStdClassObject();
  static ObjectData* AllocExceptionObject(CVarRef message);
  static ObjectData* AllocDOMExceptionObject(CVarRef message,
                                             CVarRef code);
  static ObjectData* AllocPDOExceptionObject();
  static ObjectData* AllocSoapFaultObject(CVarRef code,
                                          CVarRef message,
                                          CVarRef actor = null_variant,
                                          CVarRef detail = null_variant,
                                          CVarRef name = null_variant,
                                          CVarRef header = null_variant);
  static ObjectData* AllocGmagickExceptionObject(CVarRef message, 
		                                         CVarRef code);
  static ObjectData* AllocUserPartitionExceptionObject(CVarRef message);
  static ObjectData* AllocBucketCacheExceptionObject(CVarRef message);
};

///////////////////////////////////////////////////////////////////////////////
}

#endif
