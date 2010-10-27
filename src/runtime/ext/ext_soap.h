/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010 Facebook, Inc. (http://www.facebook.com)          |
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

#ifndef __EXT_SOAP_H__
#define __EXT_SOAP_H__

// >>>>>> Generated by idl.php. Do NOT modify. <<<<<<

#include <runtime/base/base_includes.h>
#include <system/gen/php/classes/exception.h>
#include <runtime/ext/soap/soap.h>
namespace HPHP {
///////////////////////////////////////////////////////////////////////////////

bool f_use_soap_error_handler(bool handler = true);
bool f_is_soap_fault(CVarRef fault);

///////////////////////////////////////////////////////////////////////////////
// class SoapServer

FORWARD_DECLARE_CLASS(SoapServer);
class c_SoapServer : public ExtObjectData {
 public:
  BEGIN_CLASS_MAP(SoapServer)
  END_CLASS_MAP(SoapServer)
  DECLARE_CLASS(SoapServer, SoapServer, ObjectData)
  DECLARE_INVOKES_FROM_EVAL
  ObjectData* dynCreate(CArrRef params, bool init = true);

  // need to implement
  public: c_SoapServer();
  public: ~c_SoapServer();
  public: void t___construct(CVarRef wsdl, CArrRef options = null_array);
  DECLARE_METHOD_INVOKE_HELPERS(__construct);
  public: void t_setclass(int _argc, CStrRef name, CArrRef _argv = null_array);
  DECLARE_METHOD_INVOKE_HELPERS(setclass);
  public: void t_setobject(CObjRef obj);
  DECLARE_METHOD_INVOKE_HELPERS(setobject);
  public: void t_addfunction(CVarRef func);
  DECLARE_METHOD_INVOKE_HELPERS(addfunction);
  public: Variant t_getfunctions();
  DECLARE_METHOD_INVOKE_HELPERS(getfunctions);
  public: void t_handle(CStrRef request = null_string);
  DECLARE_METHOD_INVOKE_HELPERS(handle);
  public: void t_setpersistence(int64 mode);
  DECLARE_METHOD_INVOKE_HELPERS(setpersistence);
  public: void t_fault(CVarRef code, CStrRef fault, CStrRef actor = null_string, CVarRef detail = null, CStrRef name = null_string);
  DECLARE_METHOD_INVOKE_HELPERS(fault);
  public: void t_addsoapheader(CObjRef fault);
  DECLARE_METHOD_INVOKE_HELPERS(addsoapheader);
  public: Variant t___destruct();
  DECLARE_METHOD_INVOKE_HELPERS(__destruct);

  // implemented by HPHP
  public: c_SoapServer *create(Variant wsdl, Array options = null_array);
  public: void dynConstruct(CArrRef Params);
  public: void dynConstructFromEval(Eval::VariableEnvironment &env,
                                    const Eval::FunctionCallExpression *call);
  public: void getConstructor(MethodCallPackage &mcp);
  public: virtual void destruct();

  public: int                        m_type;
  public: soapFunctions              m_soap_functions;
  public: soapClass                  m_soap_class;
  public: Object                     m_soap_object;
  public: String                     m_actor;
  public: String                     m_uri;
  public: int                        m_version;
  public: sdl                       *m_sdl;
  public: xmlCharEncodingHandlerPtr  m_encoding;
  public: Array                      m_classmap;
  public: encodeMap                 *m_typemap;
  public: int                        m_features;
  public: Array                      m_soap_headers;
  public: int                        m_send_errors;
};

///////////////////////////////////////////////////////////////////////////////
// class SoapClient

FORWARD_DECLARE_CLASS(SoapClient);
class c_SoapClient : public ExtObjectDataFlags<ObjectData::HasCall> {
 public:
  BEGIN_CLASS_MAP(SoapClient)
  END_CLASS_MAP(SoapClient)
  DECLARE_CLASS(SoapClient, SoapClient, ObjectData)
  DECLARE_INVOKES_FROM_EVAL
  ObjectData* dynCreate(CArrRef params, bool init = true);

  // need to implement
  public: c_SoapClient();
  public: ~c_SoapClient();
  public: void t___construct(CVarRef wsdl, CArrRef options = null_array);
  DECLARE_METHOD_INVOKE_HELPERS(__construct);
  public: Variant t___call(Variant name, Variant args);
  DECLARE_METHOD_INVOKE_HELPERS(__call);
  public: Variant doCall(Variant v_name, Variant v_arguments, bool fatal);
  public: Variant t___soapcall(String name, Array args, Array options = null_array, Variant input_headers = null, Variant output_headers = null);
  DECLARE_METHOD_INVOKE_HELPERS(__soapcall);
  public: Variant t___getlastrequest();
  DECLARE_METHOD_INVOKE_HELPERS(__getlastrequest);
  public: Variant t___getlastresponse();
  DECLARE_METHOD_INVOKE_HELPERS(__getlastresponse);
  public: Variant t___getlastrequestheaders();
  DECLARE_METHOD_INVOKE_HELPERS(__getlastrequestheaders);
  public: Variant t___getlastresponseheaders();
  DECLARE_METHOD_INVOKE_HELPERS(__getlastresponseheaders);
  public: Variant t___getfunctions();
  DECLARE_METHOD_INVOKE_HELPERS(__getfunctions);
  public: Variant t___gettypes();
  DECLARE_METHOD_INVOKE_HELPERS(__gettypes);
  public: Variant t___dorequest(String buf, String location, String action, int64 version, bool oneway = false);
  DECLARE_METHOD_INVOKE_HELPERS(__dorequest);
  public: Variant t___setcookie(String name, String value = null_string);
  DECLARE_METHOD_INVOKE_HELPERS(__setcookie);
  public: Variant t___setlocation(String new_location = null_string);
  DECLARE_METHOD_INVOKE_HELPERS(__setlocation);
  public: bool t___setsoapheaders(Variant headers = null);
  DECLARE_METHOD_INVOKE_HELPERS(__setsoapheaders);
  public: Variant t___destruct();
  DECLARE_METHOD_INVOKE_HELPERS(__destruct);

  // implemented by HPHP
  public: c_SoapClient *create(Variant wsdl, Array options = null_array);
  public: void dynConstruct(CArrRef Params);
  public: void dynConstructFromEval(Eval::VariableEnvironment &env,
                                    const Eval::FunctionCallExpression *call);
  public: void getConstructor(MethodCallPackage &mcp);
  public: virtual void destruct();

  public: int                         m_soap_version;
  public: sdl                        *m_sdl;
  public: xmlCharEncodingHandlerPtr   m_encoding;
  public: encodeMap                  *m_typemap;
  public: Array                       m_classmap;
  public: int                         m_features;
  public: String                      m_uri;
  public: String                      m_location;
  public: int                         m_style;
  public: int                         m_use;
  public: String                      m_login;
  public: String                      m_password;
  public: int                         m_authentication;
  public: bool                        m_digest;
  public: String                      m_proxy_host;
  public: int                         m_proxy_port;
  public: String                      m_proxy_login;
  public: String                      m_proxy_password;
  public: int                         m_connection_timeout;
  public: int                         m_max_redirect;
  public: bool                        m_use11;
  public: String                      m_user_agent;
  public: bool                        m_compression;
  public: Variant                     m_default_headers;
  public: Variant                     m_cookies;
  public: bool                        m_exceptions;
  public: Variant                     m_soap_fault;
  public: bool                        m_trace;
  public: Variant                     m_last_request;
  public: Variant                     m_last_response;
  public: Variant                     m_last_request_headers;
  public: Variant                     m_last_response_headers;

};

///////////////////////////////////////////////////////////////////////////////
// class SoapVar

FORWARD_DECLARE_CLASS(SoapVar);
class c_SoapVar : public ExtObjectData {
 public:
  BEGIN_CLASS_MAP(SoapVar)
  END_CLASS_MAP(SoapVar)
  DECLARE_CLASS(SoapVar, SoapVar, ObjectData)
  DECLARE_INVOKES_FROM_EVAL
  ObjectData* dynCreate(CArrRef params, bool init = true);

  // need to implement
  public: c_SoapVar();
  public: ~c_SoapVar();
  public: void t___construct(CVarRef data, CVarRef type, CStrRef type_name = null_string, CStrRef type_namespace = null_string, CStrRef node_name = null_string, CStrRef node_namespace = null_string);
  DECLARE_METHOD_INVOKE_HELPERS(__construct);
  public: Variant t___destruct();
  DECLARE_METHOD_INVOKE_HELPERS(__destruct);

  // implemented by HPHP
  public: c_SoapVar *create(Variant data, Variant type, String type_name = null_string, String type_namespace = null_string, String node_name = null_string, String node_namespace = null_string);
  public: void dynConstruct(CArrRef Params);
  public: void dynConstructFromEval(Eval::VariableEnvironment &env,
                                    const Eval::FunctionCallExpression *call);
  public: void getConstructor(MethodCallPackage &mcp);
  public: virtual void destruct();

  public: Variant m_value;
  public: int64   m_type;
  public: String  m_stype;
  public: String  m_ns;
  public: String  m_name;
  public: String  m_namens;
};

///////////////////////////////////////////////////////////////////////////////
// class SoapFault

FORWARD_DECLARE_CLASS(SoapFault);
class c_SoapFault : public c_Exception {
 public:
  BEGIN_CLASS_MAP(SoapFault)
  RECURSIVE_PARENT_CLASS(Exception)
  END_CLASS_MAP(SoapFault)
  DECLARE_CLASS(SoapFault, SoapFault, Exception)
  DECLARE_INVOKES_FROM_EVAL
  ObjectData* dynCreate(CArrRef params, bool init = true);

  // need to implement
  public: c_SoapFault();
  public: ~c_SoapFault();
  public: void t___construct(CVarRef code, CStrRef message, CStrRef actor = null_string, CVarRef detail = null, CStrRef name = null_string, CVarRef header = null);
  DECLARE_METHOD_INVOKE_HELPERS(__construct);
  public: String t___tostring();
  DECLARE_METHOD_INVOKE_HELPERS(__tostring);
  public: Variant t___destruct();
  DECLARE_METHOD_INVOKE_HELPERS(__destruct);

  // implemented by HPHP
  public: c_SoapFault *create(Variant code, String message, String actor = null_string, Variant detail = null, String name = null_string, Variant header = null);
  public: void dynConstruct(CArrRef Params);
  public: void dynConstructFromEval(Eval::VariableEnvironment &env,
                                    const Eval::FunctionCallExpression *call);
  public: void getConstructor(MethodCallPackage &mcp);
  public: virtual void destruct();

  public: String  m_faultstring;
  public: String  m_faultcode;
  public: String  m_faultcodens;
  public: String  m_faultactor;
  public: Variant m_detail;
  public: String  m_name;
  public: Variant m_headerfault;
};

///////////////////////////////////////////////////////////////////////////////
// class SoapParam

FORWARD_DECLARE_CLASS(SoapParam);
class c_SoapParam : public ExtObjectData {
 public:
  BEGIN_CLASS_MAP(SoapParam)
  END_CLASS_MAP(SoapParam)
  DECLARE_CLASS(SoapParam, SoapParam, ObjectData)
  DECLARE_INVOKES_FROM_EVAL
  ObjectData* dynCreate(CArrRef params, bool init = true);

  // need to implement
  public: c_SoapParam();
  public: ~c_SoapParam();
  public: void t___construct(CVarRef data, CStrRef name);
  DECLARE_METHOD_INVOKE_HELPERS(__construct);
  public: Variant t___destruct();
  DECLARE_METHOD_INVOKE_HELPERS(__destruct);

  // implemented by HPHP
  public: c_SoapParam *create(Variant data, String name);
  public: void dynConstruct(CArrRef Params);
  public: void dynConstructFromEval(Eval::VariableEnvironment &env,
                                    const Eval::FunctionCallExpression *call);
  public: void getConstructor(MethodCallPackage &mcp);
  public: virtual void destruct();

  public: String  m_name;
  public: String  m_data;
};

///////////////////////////////////////////////////////////////////////////////
// class SoapHeader

FORWARD_DECLARE_CLASS(SoapHeader);
class c_SoapHeader : public ExtObjectData {
 public:
  BEGIN_CLASS_MAP(SoapHeader)
  END_CLASS_MAP(SoapHeader)
  DECLARE_CLASS(SoapHeader, SoapHeader, ObjectData)
  DECLARE_INVOKES_FROM_EVAL
  ObjectData* dynCreate(CArrRef params, bool init = true);

  // need to implement
  public: c_SoapHeader();
  public: ~c_SoapHeader();
  public: void t___construct(CStrRef ns, CStrRef name, CVarRef data = null, bool mustunderstand = false, CVarRef actor = null);
  DECLARE_METHOD_INVOKE_HELPERS(__construct);
  public: Variant t___destruct();
  DECLARE_METHOD_INVOKE_HELPERS(__destruct);

  // implemented by HPHP
  public: c_SoapHeader *create(String ns, String name, Variant data = null, bool mustunderstand = false, Variant actor = null);
  public: void dynConstruct(CArrRef Params);
  public: void dynConstructFromEval(Eval::VariableEnvironment &env,
                                    const Eval::FunctionCallExpression *call);
  public: void getConstructor(MethodCallPackage &mcp);
  public: virtual void destruct();

  public: String  m_namespace;
  public: String  m_name;
  public: Variant m_data;
  public: bool    m_mustUnderstand;
  public: Variant m_actor;
};

///////////////////////////////////////////////////////////////////////////////
}

#endif // __EXT_SOAP_H__
