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
// @generated by HipHop Compiler

#ifndef __GENERATED_cls_ArrayIterator_hf630121e__
#define __GENERATED_cls_ArrayIterator_hf630121e__

#include <cls/ArrayAccess.h>
#include <cls/SeekableIterator.h>
#include <cls/Countable.h>

namespace HPHP {
///////////////////////////////////////////////////////////////////////////////

/* SRC: classes/iterator.php line 326 */
class c_ArrayIterator : public ExtObjectData {
  public:

  // Properties
  Variant m_arr;
  Variant m_flags;

  // Class Map
  virtual bool o_instanceof(CStrRef s) const;
  DECLARE_CLASS_COMMON(ArrayIterator, ArrayIterator)
  DECLARE_INVOKE_EX(ArrayIterator, ArrayIterator, ObjectData)

  // DECLARE_STATIC_PROP_OPS
  public:
  static Variant os_getInit(CStrRef s);
  #define OMIT_JUMP_TABLE_CLASS_STATIC_GET_ArrayIterator 1
  #define OMIT_JUMP_TABLE_CLASS_STATIC_LVAL_ArrayIterator 1
  #define OMIT_JUMP_TABLE_CLASS_CONSTANT_ArrayIterator 1

  // DECLARE_INSTANCE_PROP_OPS
  public:
  virtual void o_getArray(Array &props, bool pubOnly = false) const;
  virtual void o_setArray(CArrRef props);
  virtual Variant *o_realProp(CStrRef s, int flags,
                              CStrRef context = null_string) const;
  Variant *o_realPropPrivate(CStrRef s, int flags) const;

  // DECLARE_INSTANCE_PUBLIC_PROP_OPS
  public:
  #define OMIT_JUMP_TABLE_CLASS_realProp_PUBLIC_ArrayIterator 1

  // DECLARE_COMMON_INVOKE
  static bool os_get_call_info(MethodCallPackage &mcp, int64 hash = -1);
  #define OMIT_JUMP_TABLE_CLASS_STATIC_INVOKE_ArrayIterator 1
  virtual bool o_get_call_info(MethodCallPackage &mcp, int64 hash = -1);

  public:
  DECLARE_INVOKES_FROM_EVAL
  void init();
  public: void t___construct(Variant v_array, Variant v_flags = 0LL);
  public: c_ArrayIterator *create(Variant v_array, Variant v_flags = 0LL);
  public: ObjectData *dynCreate(CArrRef params, bool init = true);
  public: void dynConstruct(CArrRef params);
  public: void getConstructor(MethodCallPackage &mcp);
  public: void dynConstructFromEval(Eval::VariableEnvironment &env, const Eval::FunctionCallExpression *call);
  public: void t_append(CVarRef v_value);
  public: bool t_asort();
  public: int t_count();
  public: Variant t_current();
  public: Variant t_getarraycopy();
  public: Variant t_getflags();
  public: Variant t_key();
  public: bool t_ksort();
  public: Variant t_natcasesort();
  public: Variant t_natsort();
  public: Variant t_next();
  public: bool t_offsetexists(CVarRef v_index);
  public: Variant t_offsetget(Variant v_index);
  public: virtual Variant &___offsetget_lval(Variant v_index);
  public: Variant t_offsetset(CVarRef v_index, CVarRef v_newval);
  public: Variant t_offsetunset(CVarRef v_index);
  public: Variant t_rewind();
  public: void t_seek(CVarRef v_position);
  public: void t_setflags(CVarRef v_flags);
  public: bool t_uasort(CVarRef v_cmp_function);
  public: bool t_uksort(CVarRef v_cmp_function);
  public: bool t_valid();
  DECLARE_METHOD_INVOKE_HELPERS(getarraycopy);
  DECLARE_METHOD_INVOKE_HELPERS(next);
  DECLARE_METHOD_INVOKE_HELPERS(count);
  DECLARE_METHOD_INVOKE_HELPERS(natsort);
  DECLARE_METHOD_INVOKE_HELPERS(key);
  DECLARE_METHOD_INVOKE_HELPERS(valid);
  DECLARE_METHOD_INVOKE_HELPERS(append);
  DECLARE_METHOD_INVOKE_HELPERS(setflags);
  DECLARE_METHOD_INVOKE_HELPERS(__construct);
  DECLARE_METHOD_INVOKE_HELPERS(offsetexists);
  DECLARE_METHOD_INVOKE_HELPERS(uksort);
  DECLARE_METHOD_INVOKE_HELPERS(offsetget);
  DECLARE_METHOD_INVOKE_HELPERS(natcasesort);
  DECLARE_METHOD_INVOKE_HELPERS(asort);
  DECLARE_METHOD_INVOKE_HELPERS(offsetunset);
  DECLARE_METHOD_INVOKE_HELPERS(seek);
  DECLARE_METHOD_INVOKE_HELPERS(getflags);
  DECLARE_METHOD_INVOKE_HELPERS(current);
  DECLARE_METHOD_INVOKE_HELPERS(ksort);
  DECLARE_METHOD_INVOKE_HELPERS(uasort);
  DECLARE_METHOD_INVOKE_HELPERS(rewind);
  DECLARE_METHOD_INVOKE_HELPERS(offsetset);
};
extern struct ObjectStaticCallbacks cw_ArrayIterator;

///////////////////////////////////////////////////////////////////////////////
}

#endif // __GENERATED_cls_ArrayIterator_hf630121e__
