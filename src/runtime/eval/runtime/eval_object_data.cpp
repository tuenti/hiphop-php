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

#include <runtime/base/complex_types.h>
#include <runtime/eval/runtime/eval_object_data.h>
#include <runtime/eval/ast/method_statement.h>
#include <runtime/eval/ast/class_statement.h>
#include <runtime/base/hphp_system.h>
#include <runtime/eval/runtime/eval_state.h>

namespace HPHP {
namespace Eval {

/////////////////////////////////////////////////////////////////////////////
// constructor/destructor

IMPLEMENT_OBJECT_ALLOCATION_CLS(HPHP::Eval,EvalObjectData)

EvalObjectData::EvalObjectData(ClassEvalState &cls, const char* pname,
                               ObjectData* r /* = NULL */)
: DynamicObjectData(pname, r ? r : this), m_cls(cls) {
  if (pname) setRoot(root); // For ext classes
  if (r == NULL) {
    RequestEvalState::registerObject(this);
  }
  if (getMethodStatement("__get")) setAttribute(UseGet);
  if (getMethodStatement("__set")) setAttribute(UseSet);
  if (getMethodStatement("__isset")) setAttribute(UseIsset);
  if (getMethodStatement("__unset")) setAttribute(UseUnset);

  // an object can never live longer than its class
  m_class_name = m_cls.getClass()->name();

  // seems to require both
  m_invokeMcp.obj = m_invokeMcp.rootObj = this;
  // so that getClassName() will work
  m_invokeMcp.isObj = true;
}

EvalObjectData::EvalObjectData(EvalObjectData *original) :
    DynamicObjectData(0, this), m_cls(original->m_cls) {

  RequestEvalState::registerObject(this);

  if (getMethodStatement("__get")) setAttribute(UseGet);
  if (getMethodStatement("__set")) setAttribute(UseSet);

  // an object can never live longer than its class
  m_class_name = m_cls.getClass()->name();

  // seems to require both
  m_invokeMcp.obj = m_invokeMcp.rootObj = this;
  // so that getClassName() will work
  m_invokeMcp.isObj = true;

  if (ObjectData *p = original->parent.get()) {
    CountableHelper h(this);
    parent = p->clone();
    parent->setRoot(this);
    setAttributes(parent.get());
  }
  m_privates = original->m_privates;
  cloneDynamic(original);
}

void EvalObjectData::init() {
  m_cls.getClass()->initializeObject(this);
  DynamicObjectData::init();
}

void EvalObjectData::getConstructor(MethodCallPackage &mcp) {
  const MethodStatement *ms = m_cls.getConstructor();
  if (ms) {
    mcp.extra = (void*)ms;
    mcp.obj = this;
    mcp.ci = ms->getCallInfo();
  } else {
    DynamicObjectData::getConstructor(mcp);
  }
}

void EvalObjectData::destruct() {
  const MethodStatement *ms;
  incRefCount();
  if (!inCtorDtor() && (ms = getMethodStatement("__destruct"))) {
    setInDtor();
    try {
      ms->invokeInstance(Object(root), Array(), false);
    } catch (...) {
      handle_destructor_exception();
    }
  }
  decRefCount();
  if (root == this) {
    if (LIKELY(getCount() == 0)) {
      RequestEvalState::deregisterObject(this);
    }
  }
}

Array EvalObjectData::o_toArray() const {
  Array values(DynamicObjectData::o_toArray());
  Array props(Array::Create());
  m_cls.getClass()->toArray(props, values);
  if (!values.empty()) {
    props += values;
  }
  return props;
}

Variant *EvalObjectData::o_realPropHook(
  CStrRef s, int flags, CStrRef context /* = null_string */) const {
  CStrRef c = context.isNull() ? FrameInjection::GetClassName(false) : context;
  if (Variant *priv =
      const_cast<Array&>(m_privates).lvalPtr(c, flags & RealPropWrite, false)) {
    if (Variant *ret = priv->lvalPtr(s, flags & RealPropWrite, false)) {
      return ret;
    }
  }
  int mods;
  if (!(flags & RealPropUnchecked) &&
      !m_cls.getClass()->attemptPropertyAccess(s, c, mods)) {
    return NULL;
  }
  if (parent.get()) return parent->o_realProp(s, flags);
  return ObjectData::o_realPropHook(s, flags, context);
}

Variant EvalObjectData::o_getError(CStrRef prop, CStrRef context) {
  CStrRef c = context.isNull() ? FrameInjection::GetClassName(false) : context;
  int mods;
  if (!m_cls.getClass()->attemptPropertyAccess(prop, c, mods)) {
    m_cls.getClass()->failPropertyAccess(prop, c, mods);
  } else {
    DynamicObjectData::o_getError(prop, context);
  }
  return null;
}

Variant EvalObjectData::o_setError(CStrRef prop, CStrRef context) {
  CStrRef c = context.isNull() ? FrameInjection::GetClassName(false) : context;
  int mods;
  if (!m_cls.getClass()->attemptPropertyAccess(prop, c, mods)) {
    m_cls.getClass()->failPropertyAccess(prop, c, mods);
  }
  return null;
}

void EvalObjectData::o_getArray(Array &props, bool pubOnly /* = false */)
const {
  if (!pubOnly) {
    String zero("\0", 1, AttachLiteral);
    for (ArrayIter it(m_privates); !it.end(); it.next()) {
      String prefix(zero);
      prefix += it.first();
      prefix += zero;
      for (ArrayIter it2(it.second()); !it2.end(); it2.next()) {
        CVarRef v = it2.secondRef();
        if (v.isInitialized()) {
          props.lvalAt(prefix + it2.first()).setWithRef(v);
        }
      }
    }
  }
  DynamicObjectData::o_getArray(props, pubOnly);
  if (pubOnly) {
    const ClassInfo *info = ClassInfo::FindClass(o_getClassName());
    info->filterProperties(props, ClassInfo::IsProtected);
  }
}

void EvalObjectData::o_setArray(CArrRef props) {
  for (ArrayIter it(props); !it.end(); it.next()) {
    String k = it.first();
    if (!k.empty() && k.charAt(0) == '\0') {
      int subLen = k.find('\0', 1) + 1;
      String cls = k.substr(1, subLen - 2);
      String key = k.substr(subLen);
      if (cls == "*") cls = o_getClassName();
      props->load(k, o_lval(key, Variant(), cls));
    }
  }
  DynamicObjectData::o_setArray(props);
}

void EvalObjectData::o_setPrivate(CStrRef cls, CStrRef s, CVarRef v) {
  m_privates.lvalAt(cls).set(s, v, true);
}

CStrRef EvalObjectData::o_getClassNameHook() const {
  return m_class_name;
}

const MethodStatement
*EvalObjectData::getMethodStatement(const char* name) const {
  return m_cls.getMethod(name);
}

const MethodStatement* EvalObjectData::getConstructorStatement() const {
  return m_cls.getConstructor();
}

bool EvalObjectData::o_instanceof_hook(CStrRef s) const {
  return m_cls.getClass()->subclassOf(s.data()) ||
    (!parent.isNull() && parent->o_instanceof(s));
}

const CallInfo *EvalObjectData::t___invokeCallInfoHelper(void *&extra) {
  const MethodStatement *ms = getMethodStatement("__invoke");
  if (LIKELY(ms != NULL)) {
    extra = (void*) &m_invokeMcp;
    m_invokeMcp.extra = (void*) ms;
    return ms->getCallInfo();
  }
  return DynamicObjectData::t___invokeCallInfoHelper(extra);
}

bool EvalObjectData::o_get_call_info_hook(const char *clsname,
                                          MethodCallPackage &mcp,
                                          int64 hash /* = -1 */) {
  if (!clsname) {
    const ClassEvalState::MethodTable &meths = m_cls.getMethodTable();
    ClassEvalState::MethodTable::const_iterator it =
      meths.find(mcp.name->c_str());
    if (it != meths.end()) {
      if (it->second.first) {
        mcp.extra = (void*)it->second.first;
        mcp.ci = it->second.first->getCallInfo();
        return true;
      }
    }
    if (ObjectData *p = parent.get()) {
      return p->o_get_call_info(mcp, hash);
    }
  } else {
    if (m_cls.getClass()->subclassOf(clsname)) {
      bool foundClass;
      const MethodStatement *ms =
        RequestEvalState::findMethod(clsname, mcp.name->c_str(),
                                     foundClass);
      if (ms) {
        mcp.extra = (void*)ms;
        mcp.obj = this;
        mcp.ci = ms->getCallInfo();
        return true;
      }
    }
    if (ObjectData *p = parent.get()) {
      return p->o_get_call_info_ex(clsname, mcp, hash);
    }
  }
  return false;
}

Variant EvalObjectData::doCall(Variant v_name, Variant v_arguments,
                               bool fatal) {
  const MethodStatement *ms = getMethodStatement("__call");
  if (ms) {
    if (v_arguments.isNull()) {
      v_arguments = Array::Create();
    }
    return ms->invokeInstance(Object(root),
                              CREATE_VECTOR2(v_name, v_arguments), false);
  } else {
    return DynamicObjectData::doCall(v_name, v_arguments, fatal);
  }
}

Variant EvalObjectData::t___destruct() {
  const MethodStatement *ms = getMethodStatement("__destruct");
  if (ms) {
    return ms->invokeInstance(Object(root), Array(), false);
  } else {
    return DynamicObjectData::t___destruct();
  }
}
Variant EvalObjectData::t___set(Variant v_name, Variant v_value) {
  const MethodStatement *ms = getMethodStatement("__set");
  if (ms) {
    return ms->invokeInstance(Object(root),
                              CREATE_VECTOR2(v_name, withRefBind(v_value)),
        false);
  } else {
    return DynamicObjectData::t___set(v_name, withRefBind(v_value));
  }
}
Variant EvalObjectData::t___get(Variant v_name) {
  const MethodStatement *ms = getMethodStatement("__get");
  if (ms) {
    return ms->invokeInstance(Object(root), CREATE_VECTOR1(v_name), false);
  } else {
    return DynamicObjectData::t___get(v_name);
  }
}
bool EvalObjectData::t___isset(Variant v_name) {
  const MethodStatement *ms = getMethodStatement("__isset");
  if (ms) {
    return ms->invokeInstance(Object(root), CREATE_VECTOR1(v_name), false);
  } else {
    return DynamicObjectData::t___isset(v_name);
  }
}
Variant EvalObjectData::t___unset(Variant v_name) {
  const MethodStatement *ms = getMethodStatement("__unset");
  if (ms) {
    return ms->invokeInstance(Object(root), CREATE_VECTOR1(v_name), false);
  } else {
    return DynamicObjectData::t___unset(v_name);
  }
}

bool EvalObjectData::hasCall() {
  return getMethodStatement("__call") || DynamicObjectData::hasCall();
}
bool EvalObjectData::hasCallStatic() {
  return getMethodStatement("__callStatic") ||
         DynamicObjectData::hasCallStatic();
}
bool EvalObjectData::php_sleep(Variant &ret) {
  ret = t___sleep();
  return getMethodStatement("__sleep");
}

Variant EvalObjectData::t___sleep() {
  const MethodStatement *ms = getMethodStatement("__sleep");
  if (ms) {
    return ms->invokeInstance(Object(root), Array(), false);
  } else {
    return DynamicObjectData::t___sleep();
  }
}

Variant EvalObjectData::t___wakeup() {
  const MethodStatement *ms = getMethodStatement("__wakeup");
  if (ms) {
    return ms->invokeInstance(Object(root), Array(), false);
  } else {
    return DynamicObjectData::t___wakeup();
  }
}

String EvalObjectData::t___tostring() {
  const MethodStatement *ms = getMethodStatement("__tostring");
  if (ms) {
    return ms->invokeInstance(Object(root), Array(), false);
  } else {
    return DynamicObjectData::t___tostring();
  }
}
Variant EvalObjectData::t___clone() {
  const MethodStatement *ms = getMethodStatement("__clone");
  if (ms) {
    return ms->invokeInstance(Object(root), Array(), false);
  } else {
    return DynamicObjectData::t___clone();
  }
}

ObjectData* EvalObjectData::clone() {
  return NEWOBJ(EvalObjectData)(this);
}

Variant &EvalObjectData::___offsetget_lval(Variant v_name) {
  const MethodStatement *ms = getMethodStatement("offsetget");
  if (ms) {
    Variant &v = get_globals()->__lvalProxy;
    v = ms->invokeInstance(Object(root), CREATE_VECTOR1(v_name), false);
    return v;
  } else {
    return DynamicObjectData::___offsetget_lval(v_name);
  }
}

Object EvalObjectData::fiberMarshal(FiberReferenceMap &refMap) const {
  ObjectData *px = (ObjectData *)refMap.lookup((void *)this);
  if (px) return px;
  Object ret = ObjectData::fiberMarshal(refMap);
  EvalObjectData *obj = dynamic_cast<EvalObjectData*>(ret.get());
  ASSERT(obj);
  obj->m_privates = m_privates.fiberMarshal(refMap);
  return ret;
}

Object EvalObjectData::fiberUnmarshal(FiberReferenceMap &refMap) const {
  ObjectData *px = (ObjectData *)refMap.lookup((void *)this);
  if (px) return px;
  Object ret = ObjectData::fiberUnmarshal(refMap);
  EvalObjectData *obj = dynamic_cast<EvalObjectData*>(ret.get());
  ASSERT(obj);
  obj->m_privates = m_privates.fiberUnmarshal(refMap);
  return ret;
}

///////////////////////////////////////////////////////////////////////////////
}
}
