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

#include <utility>
#include <string>
#include <vector>
#include <boost/make_shared.hpp>
#include <boost/unordered_map.hpp>
#include <boost/thread/once.hpp>

#include <runtime/ext/ext_xconfig.h>
#include <xconfig/xconfig.h>

using boost::shared_ptr;
using boost::unordered_map;
using std::string;
using std::vector;
using xconfig::XConfig;
using xconfig::XConfigNode;
using xconfig::XConfigNotFound;
using xconfig::XConfigNotConnected;
using xconfig::UnixConnectionPool;

namespace HPHP {

///////////////////////////////////////////////////////////////////////////////

// connection pool is shared for all the threads
boost::once_flag s_xconfig_once = BOOST_ONCE_INIT;
UnixConnectionPool* s_xconfig_pool = 0;

void xconfig_pool_init() {
  s_xconfig_pool = new UnixConnectionPool(RuntimeOption::XConfigCacheTimeout);
}

// Get a XConfig object from connection s_xconfig_pool
shared_ptr<XConfig> getXConfigInstance(CStrRef path, CStrRef socket, bool autoreload) {
  boost::call_once(s_xconfig_once, xconfig_pool_init);
  try {
    return boost::make_shared<XConfig>(s_xconfig_pool->getConnection(path->toCPPString(), socket->toCPPString()), autoreload);
  } catch (const XConfigNotConnected& e) {
    throw Object((NEWOBJ(c_XConfigNotConnectedException)())->create(String("XConfig could not connect to ") + socket));
  }
}

///////////////////////////////////////////////////////////////////////////////

const int q_XConfig___TYPE_STRING = xconfig::TYPE_STRING;
const int q_XConfig___TYPE_BOOLEAN = xconfig::TYPE_BOOLEAN;
const int q_XConfig___TYPE_INTEGER = xconfig::TYPE_INTEGER;
const int q_XConfig___TYPE_FLOAT = xconfig::TYPE_FLOAT;
const int q_XConfig___TYPE_MAP = xconfig::TYPE_MAP;
const int q_XConfig___TYPE_SEQUENCE = xconfig::TYPE_SEQUENCE;
const StaticString q_XConfig___DEFAULT_SOCKET(xconfig::UnixConnection::DEFAULT_SOCKET);

c_XConfig::c_XConfig(const ObjectStaticCallbacks *cb) : ExtObjectData(cb)
{
}

c_XConfig::~c_XConfig()
{
}

void c_XConfig::t___construct(CStrRef path, CStrRef socket, bool autoreload)
{
  INSTANCE_METHOD_INJECTION_BUILTIN(XConfig, XConfig::__construct);
  xc = getXConfigInstance(path, socket, autoreload);
}

void c_XConfig::t_reload()
{
  INSTANCE_METHOD_INJECTION_BUILTIN(XConfig, XConfig::reload);
  xc->reload();
}

void c_XConfig::t_close()
{
  INSTANCE_METHOD_INJECTION_BUILTIN(XConfig, XConfig::close);
  xc->close();
}

XConfigNode c_XConfig::getNodeFromVariant(CVarRef key) {
  try {
    XConfigNode node;
    switch (key.getType()) {
      case KindOfObject: {
        c_XConfigNode* c_node = key.toObject().getTyped<c_XConfigNode>(true, true);
        if (c_node) {
          if (c_node->getXConfig() == xc) {
            node = c_node->getNode();
          } else {
            throw Object((NEWOBJ(c_XConfigWrongTypeException)())->create("XConfigNode object belongs to another XConfig instance"));
          }
        } else {
          throw Object((NEWOBJ(c_XConfigWrongTypeException)())->create("Expected XConfigNode object"));
        }
        break;
      }
      case KindOfStaticString:
      case KindOfString:
        node = xc->getNode(key.toString()->toCPPString());
        break;
      case KindOfArray: {
        vector<string> keys(key.toArray().size());
        int i = 0;
        for (ArrayIter iter(key.toArray()); iter; ++iter, ++i) {
          keys[i] = iter.second().toString()->toCPPString();
        }
        node = xc->getNode(keys);
        break;
      }
      default:
        throw Object((NEWOBJ(c_XConfigWrongTypeException)())->create(key));
        break;
    }
    return node;
  } catch (const XConfigNotFound& e) {
    throw Object((NEWOBJ(c_XConfigNotFoundException)())->create(String("key '") + key.toString() + "' not found"));
  } catch (const XConfigNotConnected& e) {
    throw Object((NEWOBJ(c_XConfigNotConnectedException)())->create(String("XConfig not connected")));
  }
}

Variant c_XConfig::getValue(const XConfigNode& node)
{
  switch (xc->getType(node)) {
    case xconfig::TYPE_NULL:
      return null;
    case xconfig::TYPE_BOOLEAN:
      return xc->getBool(node);
    case xconfig::TYPE_INTEGER:
      return (int64)xc->getInt(node);
    case xconfig::TYPE_FLOAT:
      return xc->getFloat(node);
    case xconfig::TYPE_STRING:
      return String(xc->getString(node));
    case xconfig::TYPE_SEQUENCE: {
      vector<XConfigNode> children = xc->getChildren(node);
      Array ret(ArrayInit(children.size()));
      for (vector<XConfigNode>::const_iterator it = children.begin(); it != children.end(); ++it) {
        ret.append(getValue(*it));
      }
      return ret;
    }
    case xconfig::TYPE_MAP: {
      vector<XConfigNode> children = xc->getChildren(node);
      Array ret(ArrayInit(children.size()));
      for (vector<XConfigNode>::const_iterator it = children.begin(); it != children.end(); ++it) {
          ret.set(String(xc->getName(*it)), getValue(*it));
      }
      return ret;
    }
    default:
      throw Object((NEWOBJ(c_XConfigWrongTypeException)())->create(String("Wrong Type")));
  }
}
Variant c_XConfig::t_getvalue(CVarRef key)
{
  INSTANCE_METHOD_INJECTION_BUILTIN(XConfig, XConfig::getvalue);
  XConfigNode node = getNodeFromVariant(key);
  return getValue(node);
}

int c_XConfig::t_gettype(CVarRef key)
{
  INSTANCE_METHOD_INJECTION_BUILTIN(XConfig, XConfig::gettype);
  XConfigNode node = getNodeFromVariant(key);
  return xc->getType(node);
}

Array c_XConfig::t_getmtime(CVarRef key)
{
  INSTANCE_METHOD_INJECTION_BUILTIN(XConfig, XConfig::getmtime);
  XConfigNode node = getNodeFromVariant(key);
  struct timespec ts = xc->getMtime(node);
  return ArrayInit(2).set(ts.tv_sec).set(ts.tv_nsec).create();
}
bool c_XConfig::t_isscalar(CVarRef key)
{
  INSTANCE_METHOD_INJECTION_BUILTIN(XConfig, XConfig::isscalar);
  XConfigNode node = getNodeFromVariant(key);
  return xc->isScalar(node);
}
bool c_XConfig::t_ismap(CVarRef key)
{
  INSTANCE_METHOD_INJECTION_BUILTIN(XConfig, XConfig::ismap);
  XConfigNode node = getNodeFromVariant(key);
  return xc->isMap(node);
}
bool c_XConfig::t_issequence(CVarRef key)
{
  INSTANCE_METHOD_INJECTION_BUILTIN(XConfig, XConfig::issequence);
  XConfigNode node = getNodeFromVariant(key);
  return xc->isSequence(node);
}
int64 c_XConfig::t_getcount(CVarRef key)
{
  INSTANCE_METHOD_INJECTION_BUILTIN(XConfig, XConfig::getcount);
  XConfigNode node = getNodeFromVariant(key);
  return xc->getCount(node);
}
Array c_XConfig::t_getmapkeys(CVarRef key)
{
  INSTANCE_METHOD_INJECTION_BUILTIN(XConfig, XConfig::getmapkeys);
  XConfigNode node = getNodeFromVariant(key);
  vector<string> keys = xc->getMapKeys(node);
  Array ret(ArrayInit(keys.size()));
  for (vector<string>::const_iterator it = keys.begin(); it != keys.end(); ++it) {
    ret.append(String(*it));
  }
  return ret;
}
Object c_XConfig::t_getnode(CVarRef key)
{
  INSTANCE_METHOD_INJECTION_BUILTIN(XConfig, XConfig::getnode);
  XConfigNode node = getNodeFromVariant(key);
  c_XConfigNode* ret = (NEWOBJ(c_XConfigNode)())->create();
  ret->_init(xc, node);
  return ret;
}

Variant c_XConfig::t___destruct()
{
  INSTANCE_METHOD_INJECTION_BUILTIN(XConfig, XConfig::__destruct);
  return null;
}

c_XConfigNode::c_XConfigNode(const ObjectStaticCallbacks *cb) : ExtObjectData(cb), node(new XConfigNode)
{
}
c_XConfigNode::~c_XConfigNode()
{
}
void c_XConfigNode::t___construct()
{
}
Variant c_XConfigNode::t___destruct()
{
  INSTANCE_METHOD_INJECTION_BUILTIN(XConfigNode, XConfigNode::__destruct);
  return null;
}
Object c_XConfigNode::t_getparent()
{
  INSTANCE_METHOD_INJECTION_BUILTIN(XConfigNode, XConfigNode::getparent);
  c_XConfigNode* ret = (NEWOBJ(c_XConfigNode)())->create();
  ret->_init(xc, xc->getParent(*node));
  return ret;
}
Array c_XConfigNode::t_getchildren()
{
  INSTANCE_METHOD_INJECTION_BUILTIN(XConfigNode, XConfigNode::getchildren);
  vector<XConfigNode> children = xc->getChildren(*node);
  Array ret(ArrayInit(children.size()));
  for (vector<XConfigNode>::const_iterator it = children.begin(); it != children.end(); ++it) {
    c_XConfigNode* c_node = (NEWOBJ(c_XConfigNode)())->create();
    c_node->_init(xc, *it);
    ret.append(c_node);
  }
  return ret;
}
String c_XConfigNode::t_getname()
{
  INSTANCE_METHOD_INJECTION_BUILTIN(XConfigNode, XConfigNode::getname);
  return xc->getName(*node);
}
void c_XConfigNode::_init(boost::shared_ptr<XConfig> xc, const XConfigNode& n)
{
  *this->node = n;
  this->xc = xc;
}
const XConfigNode& c_XConfigNode::getNode() const
{
  return *node;
}
boost::shared_ptr<XConfig> c_XConfigNode::getXConfig() const
{
  return xc;
}

c_XConfigException::c_XConfigException(const ObjectStaticCallbacks *cb) : c_Exception(cb)
{
}
c_XConfigException::~c_XConfigException()
{
}
void c_XConfigException::t___construct(CStrRef message)
{
  INSTANCE_METHOD_INJECTION_BUILTIN(XConfigException, XConfigException::__construct);
  c_Exception::t___construct(message);
}
Variant c_XConfigException::t___destruct()
{
  INSTANCE_METHOD_INJECTION_BUILTIN(XConfigException, XConfigException::__destruct);
  return null;
}

c_XConfigNotFoundException::c_XConfigNotFoundException(const ObjectStaticCallbacks *cb) : c_XConfigException(cb)
{
}
c_XConfigNotFoundException::~c_XConfigNotFoundException()
{
}
void c_XConfigNotFoundException::t___construct(CStrRef message)
{
  INSTANCE_METHOD_INJECTION_BUILTIN(XConfigNotFoundException, XConfigNotFoundException::__construct);
  c_XConfigException::t___construct(message);
}
Variant c_XConfigNotFoundException::t___destruct()
{
  INSTANCE_METHOD_INJECTION_BUILTIN(XConfigNotFoundException, XConfigNotFoundException::__destruct);
  return null;
}

c_XConfigNotConnectedException::c_XConfigNotConnectedException(const ObjectStaticCallbacks *cb) : c_XConfigException(cb)
{
}
c_XConfigNotConnectedException::~c_XConfigNotConnectedException()
{
}
void c_XConfigNotConnectedException::t___construct(CStrRef message)
{
  INSTANCE_METHOD_INJECTION_BUILTIN(XConfigNotConnectedException, XConfigNotConnectedException::__construct);
  c_XConfigException::t___construct(message);
}
Variant c_XConfigNotConnectedException::t___destruct()
{
  INSTANCE_METHOD_INJECTION_BUILTIN(XConfigNotConnectedException, XConfigNotConnectedException::__destruct);
  return null;
}

c_XConfigWrongTypeException::c_XConfigWrongTypeException(const ObjectStaticCallbacks *cb) : c_XConfigException(cb)
{
}
c_XConfigWrongTypeException::~c_XConfigWrongTypeException()
{
}
void c_XConfigWrongTypeException::t___construct(CStrRef message)
{
  INSTANCE_METHOD_INJECTION_BUILTIN(XConfigWrongTypeException, XConfigWrongTypeException::__construct);
  c_XConfigException::t___construct(message);
}
Variant c_XConfigWrongTypeException::t___destruct()
{
  INSTANCE_METHOD_INJECTION_BUILTIN(XConfigWrongTypeException, XConfigWrongTypeException::__destruct);
  return null;
}

///////////////////////////////////////////////////////////////////////////////
}  // namespace HPHP
