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

#ifndef __GENERATED_cls_pear_error_h__
#define __GENERATED_cls_pear_error_h__


namespace HPHP {
///////////////////////////////////////////////////////////////////////////////

/* SRC: classes/pear_error.php line 3 */
class c_pear_error : virtual public ObjectData {
  BEGIN_CLASS_MAP(pear_error)
  END_CLASS_MAP(pear_error)
  DECLARE_CLASS(pear_error, pear_error, ObjectData)
  DECLARE_INVOKES_FROM_EVAL
  void init();
  public: void t_pear_error(CStrRef v_message = "unknown error", CVarRef v_code = null_variant, CVarRef v_mode = null_variant, CVarRef v_options = null_variant, CVarRef v_userinfo = null_variant);
  public: ObjectData *create(CStrRef v_message = "unknown error", CVarRef v_code = null_variant, CVarRef v_mode = null_variant, CVarRef v_options = null_variant, CVarRef v_userinfo = null_variant);
  public: ObjectData *dynCreate(CArrRef params, bool init = true);
  public: void t_adduserinfo(CVarRef v_info);
  public: void t_getcallback();
  public: void t_getcode();
  public: void t_getmessage();
  public: void t_getmode();
  public: void t_getdebuginfo();
  public: void t_gettype();
  public: void t_getuserinfo();
  public: void t_getbacktrace(CVarRef v_frame = null_variant);
  public: void t_tostring();
};

///////////////////////////////////////////////////////////////////////////////
}

#endif // __GENERATED_cls_pear_error_h__
