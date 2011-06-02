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

#ifndef __EXT_DOMDOCUMENT_H__
#define __EXT_DOMDOCUMENT_H__

// >>>>>> Generated by idl.php. Do NOT modify. <<<<<<

#include <runtime/base/base_includes.h>
#include <runtime/ext/ext_domdocument_includes.h>
namespace HPHP {
///////////////////////////////////////////////////////////////////////////////

Variant f_dom_document_create_element(CVarRef obj, CStrRef name, CStrRef value = null_string);
Variant f_dom_document_create_document_fragment(CVarRef obj);
Variant f_dom_document_create_text_node(CVarRef obj, CStrRef data);
Variant f_dom_document_create_comment(CVarRef obj, CStrRef data);
Variant f_dom_document_create_cdatasection(CVarRef obj, CStrRef data);
Variant f_dom_document_create_processing_instruction(CVarRef obj, CStrRef target, CStrRef data = null_string);
Variant f_dom_document_create_attribute(CVarRef obj, CStrRef name);
Variant f_dom_document_create_entity_reference(CVarRef obj, CStrRef name);
Variant f_dom_document_get_elements_by_tag_name(CVarRef obj, CStrRef name);
Variant f_dom_document_import_node(CVarRef obj, CObjRef importednode, bool deep = false);
Variant f_dom_document_create_element_ns(CVarRef obj, CStrRef namespaceuri, CStrRef qualifiedname, CStrRef value = null_string);
Variant f_dom_document_create_attribute_ns(CVarRef obj, CStrRef namespaceuri, CStrRef qualifiedname);
Variant f_dom_document_get_elements_by_tag_name_ns(CVarRef obj, CStrRef namespaceuri, CStrRef localname);
Variant f_dom_document_get_element_by_id(CVarRef obj, CStrRef elementid);
Variant f_dom_document_normalize_document(CVarRef obj);
Variant f_dom_document_save(CVarRef obj, CStrRef file, int64 options = 0);
Variant f_dom_document_savexml(CVarRef obj, CObjRef node = null_object, int64 options = 0);
Variant f_dom_document_validate(CVarRef obj);
Variant f_dom_document_xinclude(CVarRef obj, int64 options = 0);
Variant f_dom_document_save_html(CVarRef obj);
Variant f_dom_document_save_html_file(CVarRef obj, CStrRef file);
Variant f_dom_document_schema_validate_file(CVarRef obj, CStrRef filename);
Variant f_dom_document_schema_validate_xml(CVarRef obj, CStrRef source);
Variant f_dom_document_relaxng_validate_file(CVarRef obj, CStrRef filename);
Variant f_dom_document_relaxng_validate_xml(CVarRef obj, CStrRef source);
Variant f_dom_node_insert_before(CVarRef obj, CObjRef newnode, CObjRef refnode = null);
Variant f_dom_node_replace_child(CVarRef obj, CObjRef newchildobj, CObjRef oldchildobj);
Variant f_dom_node_remove_child(CVarRef obj, CObjRef node);
Variant f_dom_node_append_child(CVarRef obj, CObjRef newnode);
Variant f_dom_node_has_child_nodes(CVarRef obj);
Variant f_dom_node_clone_node(CVarRef obj, bool deep = false);
Variant f_dom_node_normalize(CVarRef obj);
Variant f_dom_node_is_supported(CVarRef obj, CStrRef feature, CStrRef version);
Variant f_dom_node_has_attributes(CVarRef obj);
Variant f_dom_node_is_same_node(CVarRef obj, CObjRef node);
Variant f_dom_node_lookup_prefix(CVarRef obj, CStrRef prefix);
Variant f_dom_node_is_default_namespace(CVarRef obj, CStrRef namespaceuri);
Variant f_dom_node_lookup_namespace_uri(CVarRef obj, CStrRef namespaceuri);
Variant f_dom_nodelist_item(CVarRef obj, int64 index);
Variant f_dom_namednodemap_get_named_item(CVarRef obj, CStrRef name);
Variant f_dom_namednodemap_item(CVarRef obj, int64 index);
Variant f_dom_namednodemap_get_named_item_ns(CVarRef obj, CStrRef namespaceuri, CStrRef localname);
Variant f_dom_characterdata_substring_data(CVarRef obj, int64 offset, int64 count);
Variant f_dom_characterdata_append_data(CVarRef obj, CStrRef arg);
Variant f_dom_characterdata_insert_data(CVarRef obj, int64 offset, CStrRef data);
Variant f_dom_characterdata_delete_data(CVarRef obj, int64 offset, int64 count);
Variant f_dom_characterdata_replace_data(CVarRef obj, int64 offset, int64 count, CStrRef data);
Variant f_dom_attr_is_id(CVarRef obj);
Variant f_dom_element_get_attribute(CVarRef obj, CStrRef name);
Variant f_dom_element_set_attribute(CVarRef obj, CStrRef name, CStrRef value);
Variant f_dom_element_remove_attribute(CVarRef obj, CStrRef name);
Variant f_dom_element_get_attribute_node(CVarRef obj, CStrRef name);
Variant f_dom_element_set_attribute_node(CVarRef obj, CObjRef newattr);
Variant f_dom_element_remove_attribute_node(CVarRef obj, CObjRef oldattr);
Variant f_dom_element_get_elements_by_tag_name(CVarRef obj, CStrRef name);
Variant f_dom_element_get_attribute_ns(CVarRef obj, CStrRef namespaceuri, CStrRef localname);
Variant f_dom_element_set_attribute_ns(CVarRef obj, CStrRef namespaceuri, CStrRef name, CStrRef value);
Variant f_dom_element_remove_attribute_ns(CVarRef obj, CStrRef namespaceuri, CStrRef localname);
Variant f_dom_element_get_attribute_node_ns(CVarRef obj, CStrRef namespaceuri, CStrRef localname);
Variant f_dom_element_set_attribute_node_ns(CVarRef obj, CObjRef newattr);
Variant f_dom_element_get_elements_by_tag_name_ns(CVarRef obj, CStrRef namespaceuri, CStrRef localname);
Variant f_dom_element_has_attribute(CVarRef obj, CStrRef name);
Variant f_dom_element_has_attribute_ns(CVarRef obj, CStrRef namespaceuri, CStrRef localname);
Variant f_dom_element_set_id_attribute(CVarRef obj, CStrRef name, bool isid);
Variant f_dom_element_set_id_attribute_ns(CVarRef obj, CStrRef namespaceuri, CStrRef localname, bool isid);
Variant f_dom_element_set_id_attribute_node(CVarRef obj, CObjRef idattr, bool isid);
Variant f_dom_text_split_text(CVarRef obj, int64 offset);
Variant f_dom_text_is_whitespace_in_element_content(CVarRef obj);
Variant f_dom_xpath_register_ns(CVarRef obj, CStrRef prefix, CStrRef uri);
Variant f_dom_xpath_query(CVarRef obj, CStrRef expr, CObjRef context = null_object);
Variant f_dom_xpath_evaluate(CVarRef obj, CStrRef expr, CObjRef context = null_object);
Variant f_dom_xpath_register_php_functions(CVarRef obj, CVarRef funcs = null);

///////////////////////////////////////////////////////////////////////////////
// class DOMNode

FORWARD_DECLARE_CLASS_BUILTIN(DOMNode);
class c_DOMNode : public ExtObjectDataFlags<ObjectData::UseGet|ObjectData::UseSet> {
 public:
  BEGIN_CLASS_MAP(DOMNode)
  END_CLASS_MAP(DOMNode)
  DECLARE_CLASS(DOMNode, DOMNode, ObjectData)

  // need to implement
  public: c_DOMNode();
  public: ~c_DOMNode();
  public: void t___construct();
  DECLARE_METHOD_INVOKE_HELPERS(__construct);
  public: Variant t_appendchild(CObjRef newnode);
  DECLARE_METHOD_INVOKE_HELPERS(appendchild);
  public: Variant t_clonenode(bool deep = false);
  DECLARE_METHOD_INVOKE_HELPERS(clonenode);
  public: int64 t_getlineno();
  DECLARE_METHOD_INVOKE_HELPERS(getlineno);
  public: bool t_hasattributes();
  DECLARE_METHOD_INVOKE_HELPERS(hasattributes);
  public: bool t_haschildnodes();
  DECLARE_METHOD_INVOKE_HELPERS(haschildnodes);
  public: Variant t_insertbefore(CObjRef newnode, CObjRef refnode = null);
  DECLARE_METHOD_INVOKE_HELPERS(insertbefore);
  public: bool t_isdefaultnamespace(CStrRef namespaceuri);
  DECLARE_METHOD_INVOKE_HELPERS(isdefaultnamespace);
  public: bool t_issamenode(CObjRef node);
  DECLARE_METHOD_INVOKE_HELPERS(issamenode);
  public: bool t_issupported(CStrRef feature, CStrRef version);
  DECLARE_METHOD_INVOKE_HELPERS(issupported);
  public: Variant t_lookupnamespaceuri(CStrRef namespaceuri);
  DECLARE_METHOD_INVOKE_HELPERS(lookupnamespaceuri);
  public: Variant t_lookupprefix(CStrRef prefix);
  DECLARE_METHOD_INVOKE_HELPERS(lookupprefix);
  public: void t_normalize();
  DECLARE_METHOD_INVOKE_HELPERS(normalize);
  public: Variant t_removechild(CObjRef node);
  DECLARE_METHOD_INVOKE_HELPERS(removechild);
  public: Variant t_replacechild(CObjRef newchildobj, CObjRef oldchildobj);
  DECLARE_METHOD_INVOKE_HELPERS(replacechild);
  public: Variant t_c14n(bool exclusive = false, bool with_comments = false, CVarRef xpath = null, CVarRef ns_prefixes = null);
  DECLARE_METHOD_INVOKE_HELPERS(c14n);
  public: Variant t_c14nfile(CStrRef uri, bool exclusive = false, bool with_comments = false, CVarRef xpath = null, CVarRef ns_prefixes = null);
  DECLARE_METHOD_INVOKE_HELPERS(c14nfile);
  public: Variant t_getnodepath();
  DECLARE_METHOD_INVOKE_HELPERS(getnodepath);
  public: Variant t___get(Variant name);
  DECLARE_METHOD_INVOKE_HELPERS(__get);
  public: Variant t___set(Variant name, Variant value);
  DECLARE_METHOD_INVOKE_HELPERS(__set);
  public: bool t___isset(Variant name);
  DECLARE_METHOD_INVOKE_HELPERS(__isset);
  public: Variant t___destruct();
  DECLARE_METHOD_INVOKE_HELPERS(__destruct);

  // implemented by HPHP
  public: c_DOMNode *create();
  public: void dynConstruct(CArrRef Params);
  public: void getConstructor(MethodCallPackage &mcp);
  public: virtual void destruct();

public:
  virtual ObjectData *clone();
  virtual p_DOMDocument doc() { return m_doc;}
  p_DOMDocument m_doc;
  xmlNodePtr m_node;
};

///////////////////////////////////////////////////////////////////////////////
// class DOMAttr

FORWARD_DECLARE_CLASS_BUILTIN(DOMAttr);
class c_DOMAttr : public c_DOMNode {
 public:
  BEGIN_CLASS_MAP(DOMAttr)
  RECURSIVE_PARENT_CLASS(DOMNode)
  END_CLASS_MAP(DOMAttr)
  DECLARE_CLASS(DOMAttr, DOMAttr, DOMNode)

  // need to implement
  public: c_DOMAttr();
  public: ~c_DOMAttr();
  public: void t___construct(CStrRef name, CStrRef value = null_string);
  DECLARE_METHOD_INVOKE_HELPERS(__construct);
  public: bool t_isid();
  DECLARE_METHOD_INVOKE_HELPERS(isid);
  public: Variant t___get(Variant name);
  DECLARE_METHOD_INVOKE_HELPERS(__get);
  public: Variant t___set(Variant name, Variant value);
  DECLARE_METHOD_INVOKE_HELPERS(__set);
  public: bool t___isset(Variant name);
  DECLARE_METHOD_INVOKE_HELPERS(__isset);
  public: Variant t___destruct();
  DECLARE_METHOD_INVOKE_HELPERS(__destruct);

  // implemented by HPHP
  public: c_DOMAttr *create(String name, String value = null_string);
  public: void dynConstruct(CArrRef Params);
  public: void getConstructor(MethodCallPackage &mcp);
  public: virtual void destruct();

};

///////////////////////////////////////////////////////////////////////////////
// class DOMCharacterData

FORWARD_DECLARE_CLASS_BUILTIN(DOMCharacterData);
class c_DOMCharacterData : public c_DOMNode {
 public:
  BEGIN_CLASS_MAP(DOMCharacterData)
  RECURSIVE_PARENT_CLASS(DOMNode)
  END_CLASS_MAP(DOMCharacterData)
  DECLARE_CLASS(DOMCharacterData, DOMCharacterData, DOMNode)

  // need to implement
  public: c_DOMCharacterData();
  public: ~c_DOMCharacterData();
  public: void t___construct();
  DECLARE_METHOD_INVOKE_HELPERS(__construct);
  public: bool t_appenddata(CStrRef arg);
  DECLARE_METHOD_INVOKE_HELPERS(appenddata);
  public: bool t_deletedata(int64 offset, int64 count);
  DECLARE_METHOD_INVOKE_HELPERS(deletedata);
  public: bool t_insertdata(int64 offset, CStrRef data);
  DECLARE_METHOD_INVOKE_HELPERS(insertdata);
  public: bool t_replacedata(int64 offset, int64 count, CStrRef data);
  DECLARE_METHOD_INVOKE_HELPERS(replacedata);
  public: String t_substringdata(int64 offset, int64 count);
  DECLARE_METHOD_INVOKE_HELPERS(substringdata);
  public: Variant t___get(Variant name);
  DECLARE_METHOD_INVOKE_HELPERS(__get);
  public: Variant t___set(Variant name, Variant value);
  DECLARE_METHOD_INVOKE_HELPERS(__set);
  public: bool t___isset(Variant name);
  DECLARE_METHOD_INVOKE_HELPERS(__isset);
  public: Variant t___destruct();
  DECLARE_METHOD_INVOKE_HELPERS(__destruct);

  // implemented by HPHP
  public: c_DOMCharacterData *create();
  public: void dynConstruct(CArrRef Params);
  public: void getConstructor(MethodCallPackage &mcp);
  public: virtual void destruct();

};

///////////////////////////////////////////////////////////////////////////////
// class DOMComment

FORWARD_DECLARE_CLASS_BUILTIN(DOMComment);
class c_DOMComment : public c_DOMCharacterData {
 public:
  BEGIN_CLASS_MAP(DOMComment)
  RECURSIVE_PARENT_CLASS(DOMCharacterData)
  END_CLASS_MAP(DOMComment)
  DECLARE_CLASS(DOMComment, DOMComment, DOMCharacterData)

  // need to implement
  public: c_DOMComment();
  public: ~c_DOMComment();
  public: void t___construct(CStrRef value = null_string);
  DECLARE_METHOD_INVOKE_HELPERS(__construct);
  public: Variant t___destruct();
  DECLARE_METHOD_INVOKE_HELPERS(__destruct);

  // implemented by HPHP
  public: c_DOMComment *create(String value = null_string);
  public: void dynConstruct(CArrRef Params);
  public: void getConstructor(MethodCallPackage &mcp);
  public: virtual void destruct();

};

///////////////////////////////////////////////////////////////////////////////
// class DOMText

FORWARD_DECLARE_CLASS_BUILTIN(DOMText);
class c_DOMText : public c_DOMCharacterData {
 public:
  BEGIN_CLASS_MAP(DOMText)
  RECURSIVE_PARENT_CLASS(DOMCharacterData)
  END_CLASS_MAP(DOMText)
  DECLARE_CLASS(DOMText, DOMText, DOMCharacterData)

  // need to implement
  public: c_DOMText();
  public: ~c_DOMText();
  public: void t___construct(CStrRef value = null_string);
  DECLARE_METHOD_INVOKE_HELPERS(__construct);
  public: bool t_iswhitespaceinelementcontent();
  DECLARE_METHOD_INVOKE_HELPERS(iswhitespaceinelementcontent);
  public: Variant t_splittext(int64 offset);
  DECLARE_METHOD_INVOKE_HELPERS(splittext);
  public: Variant t___get(Variant name);
  DECLARE_METHOD_INVOKE_HELPERS(__get);
  public: Variant t___set(Variant name, Variant value);
  DECLARE_METHOD_INVOKE_HELPERS(__set);
  public: bool t___isset(Variant name);
  DECLARE_METHOD_INVOKE_HELPERS(__isset);
  public: Variant t___destruct();
  DECLARE_METHOD_INVOKE_HELPERS(__destruct);

  // implemented by HPHP
  public: c_DOMText *create(String value = null_string);
  public: void dynConstruct(CArrRef Params);
  public: void getConstructor(MethodCallPackage &mcp);
  public: virtual void destruct();

};

///////////////////////////////////////////////////////////////////////////////
// class DOMCDATASection

FORWARD_DECLARE_CLASS_BUILTIN(DOMCDATASection);
class c_DOMCDATASection : public c_DOMText {
 public:
  BEGIN_CLASS_MAP(DOMCDATASection)
  RECURSIVE_PARENT_CLASS(DOMText)
  END_CLASS_MAP(DOMCDATASection)
  DECLARE_CLASS(DOMCDATASection, DOMCDATASection, DOMText)

  // need to implement
  public: c_DOMCDATASection();
  public: ~c_DOMCDATASection();
  public: void t___construct(CStrRef value);
  DECLARE_METHOD_INVOKE_HELPERS(__construct);
  public: Variant t___destruct();
  DECLARE_METHOD_INVOKE_HELPERS(__destruct);

  // implemented by HPHP
  public: c_DOMCDATASection *create(String value);
  public: void dynConstruct(CArrRef Params);
  public: void getConstructor(MethodCallPackage &mcp);
  public: virtual void destruct();

};

///////////////////////////////////////////////////////////////////////////////
// class DOMDocument

FORWARD_DECLARE_CLASS_BUILTIN(DOMDocument);
class c_DOMDocument : public c_DOMNode, public Sweepable {
 public:
  BEGIN_CLASS_MAP(DOMDocument)
  RECURSIVE_PARENT_CLASS(DOMNode)
  END_CLASS_MAP(DOMDocument)
  DECLARE_CLASS(DOMDocument, DOMDocument, DOMNode)

  // need to implement
  public: c_DOMDocument();
  public: ~c_DOMDocument();
  public: void t___construct(CStrRef version = null_string, CStrRef encoding = null_string);
  DECLARE_METHOD_INVOKE_HELPERS(__construct);
  public: Variant t_createattribute(CStrRef name);
  DECLARE_METHOD_INVOKE_HELPERS(createattribute);
  public: Variant t_createattributens(CStrRef namespaceuri, CStrRef qualifiedname);
  DECLARE_METHOD_INVOKE_HELPERS(createattributens);
  public: Variant t_createcdatasection(CStrRef data);
  DECLARE_METHOD_INVOKE_HELPERS(createcdatasection);
  public: Variant t_createcomment(CStrRef data);
  DECLARE_METHOD_INVOKE_HELPERS(createcomment);
  public: Variant t_createdocumentfragment();
  DECLARE_METHOD_INVOKE_HELPERS(createdocumentfragment);
  public: Variant t_createelement(CStrRef name, CStrRef value = null_string);
  DECLARE_METHOD_INVOKE_HELPERS(createelement);
  public: Variant t_createelementns(CStrRef namespaceuri, CStrRef qualifiedname, CStrRef value = null_string);
  DECLARE_METHOD_INVOKE_HELPERS(createelementns);
  public: Variant t_createentityreference(CStrRef name);
  DECLARE_METHOD_INVOKE_HELPERS(createentityreference);
  public: Variant t_createprocessinginstruction(CStrRef target, CStrRef data = null_string);
  DECLARE_METHOD_INVOKE_HELPERS(createprocessinginstruction);
  public: Variant t_createtextnode(CStrRef data);
  DECLARE_METHOD_INVOKE_HELPERS(createtextnode);
  public: Variant t_getelementbyid(CStrRef elementid);
  DECLARE_METHOD_INVOKE_HELPERS(getelementbyid);
  public: Variant t_getelementsbytagname(CStrRef name);
  DECLARE_METHOD_INVOKE_HELPERS(getelementsbytagname);
  public: Variant t_getelementsbytagnamens(CStrRef namespaceuri, CStrRef localname);
  DECLARE_METHOD_INVOKE_HELPERS(getelementsbytagnamens);
  public: Variant t_importnode(CObjRef importednode, bool deep = false);
  DECLARE_METHOD_INVOKE_HELPERS(importnode);
  public: Variant t_load(CStrRef filename, int64 options = 0);
  DECLARE_METHOD_INVOKE_HELPERS(load);
  public: Variant t_loadhtml(CStrRef source);
  DECLARE_METHOD_INVOKE_HELPERS(loadhtml);
  public: Variant t_loadhtmlfile(CStrRef filename);
  DECLARE_METHOD_INVOKE_HELPERS(loadhtmlfile);
  public: Variant t_loadxml(CStrRef source, int64 options = 0);
  DECLARE_METHOD_INVOKE_HELPERS(loadxml);
  public: void t_normalizedocument();
  DECLARE_METHOD_INVOKE_HELPERS(normalizedocument);
  public: bool t_registernodeclass(CStrRef baseclass, CStrRef extendedclass);
  DECLARE_METHOD_INVOKE_HELPERS(registernodeclass);
  public: bool t_relaxngvalidate(CStrRef filename);
  DECLARE_METHOD_INVOKE_HELPERS(relaxngvalidate);
  public: bool t_relaxngvalidatesource(CStrRef source);
  DECLARE_METHOD_INVOKE_HELPERS(relaxngvalidatesource);
  public: Variant t_save(CStrRef file, int64 options = 0);
  DECLARE_METHOD_INVOKE_HELPERS(save);
  public: Variant t_savehtml();
  DECLARE_METHOD_INVOKE_HELPERS(savehtml);
  public: Variant t_savehtmlfile(CStrRef file);
  DECLARE_METHOD_INVOKE_HELPERS(savehtmlfile);
  public: Variant t_savexml(CObjRef node = null_object, int64 options = 0);
  DECLARE_METHOD_INVOKE_HELPERS(savexml);
  public: bool t_schemavalidate(CStrRef filename);
  DECLARE_METHOD_INVOKE_HELPERS(schemavalidate);
  public: bool t_schemavalidatesource(CStrRef source);
  DECLARE_METHOD_INVOKE_HELPERS(schemavalidatesource);
  public: bool t_validate();
  DECLARE_METHOD_INVOKE_HELPERS(validate);
  public: Variant t_xinclude(int64 options = 0);
  DECLARE_METHOD_INVOKE_HELPERS(xinclude);
  public: Variant t___get(Variant name);
  DECLARE_METHOD_INVOKE_HELPERS(__get);
  public: Variant t___set(Variant name, Variant value);
  DECLARE_METHOD_INVOKE_HELPERS(__set);
  public: bool t___isset(Variant name);
  DECLARE_METHOD_INVOKE_HELPERS(__isset);
  public: Variant t___destruct();
  DECLARE_METHOD_INVOKE_HELPERS(__destruct);

  // implemented by HPHP
  public: c_DOMDocument *create(String version = null_string, String encoding = null_string);
  public: void dynConstruct(CArrRef Params);
  public: void getConstructor(MethodCallPackage &mcp);
  public: virtual void destruct();

public:
  virtual p_DOMDocument doc() { return this;}
  bool m_formatoutput;
  bool m_validateonparse;
  bool m_resolveexternals;
  bool m_preservewhitespace;
  bool m_substituteentities;
  bool m_stricterror;
  bool m_recover;
  Array m_classmap;
  std::auto_ptr<XmlNodeSet> m_orphans;
  bool m_owner;
};

///////////////////////////////////////////////////////////////////////////////
// class DOMDocumentFragment

FORWARD_DECLARE_CLASS_BUILTIN(DOMDocumentFragment);
class c_DOMDocumentFragment : public c_DOMNode {
 public:
  BEGIN_CLASS_MAP(DOMDocumentFragment)
  RECURSIVE_PARENT_CLASS(DOMNode)
  END_CLASS_MAP(DOMDocumentFragment)
  DECLARE_CLASS(DOMDocumentFragment, DOMDocumentFragment, DOMNode)

  // need to implement
  public: c_DOMDocumentFragment();
  public: ~c_DOMDocumentFragment();
  public: void t___construct();
  DECLARE_METHOD_INVOKE_HELPERS(__construct);
  public: bool t_appendxml(CStrRef data);
  DECLARE_METHOD_INVOKE_HELPERS(appendxml);
  public: Variant t___destruct();
  DECLARE_METHOD_INVOKE_HELPERS(__destruct);

  // implemented by HPHP
  public: c_DOMDocumentFragment *create();
  public: void dynConstruct(CArrRef Params);
  public: void getConstructor(MethodCallPackage &mcp);
  public: virtual void destruct();

};

///////////////////////////////////////////////////////////////////////////////
// class DOMDocumentType

FORWARD_DECLARE_CLASS_BUILTIN(DOMDocumentType);
class c_DOMDocumentType : public c_DOMNode {
 public:
  BEGIN_CLASS_MAP(DOMDocumentType)
  RECURSIVE_PARENT_CLASS(DOMNode)
  END_CLASS_MAP(DOMDocumentType)
  DECLARE_CLASS(DOMDocumentType, DOMDocumentType, DOMNode)

  // need to implement
  public: c_DOMDocumentType();
  public: ~c_DOMDocumentType();
  public: void t___construct();
  DECLARE_METHOD_INVOKE_HELPERS(__construct);
  public: Variant t___get(Variant name);
  DECLARE_METHOD_INVOKE_HELPERS(__get);
  public: Variant t___set(Variant name, Variant value);
  DECLARE_METHOD_INVOKE_HELPERS(__set);
  public: bool t___isset(Variant name);
  DECLARE_METHOD_INVOKE_HELPERS(__isset);
  public: Variant t___destruct();
  DECLARE_METHOD_INVOKE_HELPERS(__destruct);

  // implemented by HPHP
  public: c_DOMDocumentType *create();
  public: void dynConstruct(CArrRef Params);
  public: void getConstructor(MethodCallPackage &mcp);
  public: virtual void destruct();

};

///////////////////////////////////////////////////////////////////////////////
// class DOMElement

FORWARD_DECLARE_CLASS_BUILTIN(DOMElement);
class c_DOMElement : public c_DOMNode {
 public:
  BEGIN_CLASS_MAP(DOMElement)
  RECURSIVE_PARENT_CLASS(DOMNode)
  END_CLASS_MAP(DOMElement)
  DECLARE_CLASS(DOMElement, DOMElement, DOMNode)

  // need to implement
  public: c_DOMElement();
  public: ~c_DOMElement();
  public: void t___construct(CStrRef name, CStrRef value = null_string, CStrRef namespaceuri = null_string);
  DECLARE_METHOD_INVOKE_HELPERS(__construct);
  public: String t_getattribute(CStrRef name);
  DECLARE_METHOD_INVOKE_HELPERS(getattribute);
  public: Variant t_getattributenode(CStrRef name);
  DECLARE_METHOD_INVOKE_HELPERS(getattributenode);
  public: Object t_getattributenodens(CStrRef namespaceuri, CStrRef localname);
  DECLARE_METHOD_INVOKE_HELPERS(getattributenodens);
  public: String t_getattributens(CStrRef namespaceuri, CStrRef localname);
  DECLARE_METHOD_INVOKE_HELPERS(getattributens);
  public: Object t_getelementsbytagname(CStrRef name);
  DECLARE_METHOD_INVOKE_HELPERS(getelementsbytagname);
  public: Object t_getelementsbytagnamens(CStrRef namespaceuri, CStrRef localname);
  DECLARE_METHOD_INVOKE_HELPERS(getelementsbytagnamens);
  public: bool t_hasattribute(CStrRef name);
  DECLARE_METHOD_INVOKE_HELPERS(hasattribute);
  public: bool t_hasattributens(CStrRef namespaceuri, CStrRef localname);
  DECLARE_METHOD_INVOKE_HELPERS(hasattributens);
  public: bool t_removeattribute(CStrRef name);
  DECLARE_METHOD_INVOKE_HELPERS(removeattribute);
  public: Variant t_removeattributenode(CObjRef oldattr);
  DECLARE_METHOD_INVOKE_HELPERS(removeattributenode);
  public: Variant t_removeattributens(CStrRef namespaceuri, CStrRef localname);
  DECLARE_METHOD_INVOKE_HELPERS(removeattributens);
  public: Variant t_setattribute(CStrRef name, CStrRef value);
  DECLARE_METHOD_INVOKE_HELPERS(setattribute);
  public: Variant t_setattributenode(CObjRef newattr);
  DECLARE_METHOD_INVOKE_HELPERS(setattributenode);
  public: Variant t_setattributenodens(CObjRef newattr);
  DECLARE_METHOD_INVOKE_HELPERS(setattributenodens);
  public: Variant t_setattributens(CStrRef namespaceuri, CStrRef name, CStrRef value);
  DECLARE_METHOD_INVOKE_HELPERS(setattributens);
  public: Variant t_setidattribute(CStrRef name, bool isid);
  DECLARE_METHOD_INVOKE_HELPERS(setidattribute);
  public: Variant t_setidattributenode(CObjRef idattr, bool isid);
  DECLARE_METHOD_INVOKE_HELPERS(setidattributenode);
  public: Variant t_setidattributens(CStrRef namespaceuri, CStrRef localname, bool isid);
  DECLARE_METHOD_INVOKE_HELPERS(setidattributens);
  public: Variant t___get(Variant name);
  DECLARE_METHOD_INVOKE_HELPERS(__get);
  public: Variant t___set(Variant name, Variant value);
  DECLARE_METHOD_INVOKE_HELPERS(__set);
  public: bool t___isset(Variant name);
  DECLARE_METHOD_INVOKE_HELPERS(__isset);
  public: Variant t___destruct();
  DECLARE_METHOD_INVOKE_HELPERS(__destruct);

  // implemented by HPHP
  public: c_DOMElement *create(String name, String value = null_string, String namespaceuri = null_string);
  public: void dynConstruct(CArrRef Params);
  public: void getConstructor(MethodCallPackage &mcp);
  public: virtual void destruct();

};

///////////////////////////////////////////////////////////////////////////////
// class DOMEntity

FORWARD_DECLARE_CLASS_BUILTIN(DOMEntity);
class c_DOMEntity : public c_DOMNode {
 public:
  BEGIN_CLASS_MAP(DOMEntity)
  RECURSIVE_PARENT_CLASS(DOMNode)
  END_CLASS_MAP(DOMEntity)
  DECLARE_CLASS(DOMEntity, DOMEntity, DOMNode)

  // need to implement
  public: c_DOMEntity();
  public: ~c_DOMEntity();
  public: void t___construct();
  DECLARE_METHOD_INVOKE_HELPERS(__construct);
  public: Variant t___get(Variant name);
  DECLARE_METHOD_INVOKE_HELPERS(__get);
  public: Variant t___set(Variant name, Variant value);
  DECLARE_METHOD_INVOKE_HELPERS(__set);
  public: bool t___isset(Variant name);
  DECLARE_METHOD_INVOKE_HELPERS(__isset);
  public: Variant t___destruct();
  DECLARE_METHOD_INVOKE_HELPERS(__destruct);

  // implemented by HPHP
  public: c_DOMEntity *create();
  public: void dynConstruct(CArrRef Params);
  public: void getConstructor(MethodCallPackage &mcp);
  public: virtual void destruct();

};

///////////////////////////////////////////////////////////////////////////////
// class DOMEntityReference

FORWARD_DECLARE_CLASS_BUILTIN(DOMEntityReference);
class c_DOMEntityReference : public c_DOMNode {
 public:
  BEGIN_CLASS_MAP(DOMEntityReference)
  RECURSIVE_PARENT_CLASS(DOMNode)
  END_CLASS_MAP(DOMEntityReference)
  DECLARE_CLASS(DOMEntityReference, DOMEntityReference, DOMNode)

  // need to implement
  public: c_DOMEntityReference();
  public: ~c_DOMEntityReference();
  public: void t___construct(CStrRef name);
  DECLARE_METHOD_INVOKE_HELPERS(__construct);
  public: Variant t___destruct();
  DECLARE_METHOD_INVOKE_HELPERS(__destruct);

  // implemented by HPHP
  public: c_DOMEntityReference *create(String name);
  public: void dynConstruct(CArrRef Params);
  public: void getConstructor(MethodCallPackage &mcp);
  public: virtual void destruct();

};

///////////////////////////////////////////////////////////////////////////////
// class DOMNotation

FORWARD_DECLARE_CLASS_BUILTIN(DOMNotation);
class c_DOMNotation : public c_DOMNode {
 public:
  BEGIN_CLASS_MAP(DOMNotation)
  RECURSIVE_PARENT_CLASS(DOMNode)
  END_CLASS_MAP(DOMNotation)
  DECLARE_CLASS(DOMNotation, DOMNotation, DOMNode)

  // need to implement
  public: c_DOMNotation();
  public: ~c_DOMNotation();
  public: void t___construct();
  DECLARE_METHOD_INVOKE_HELPERS(__construct);
  public: Variant t___get(Variant name);
  DECLARE_METHOD_INVOKE_HELPERS(__get);
  public: Variant t___set(Variant name, Variant value);
  DECLARE_METHOD_INVOKE_HELPERS(__set);
  public: bool t___isset(Variant name);
  DECLARE_METHOD_INVOKE_HELPERS(__isset);
  public: Variant t___destruct();
  DECLARE_METHOD_INVOKE_HELPERS(__destruct);

  // implemented by HPHP
  public: c_DOMNotation *create();
  public: void dynConstruct(CArrRef Params);
  public: void getConstructor(MethodCallPackage &mcp);
  public: virtual void destruct();

};

///////////////////////////////////////////////////////////////////////////////
// class DOMProcessingInstruction

FORWARD_DECLARE_CLASS_BUILTIN(DOMProcessingInstruction);
class c_DOMProcessingInstruction : public c_DOMNode {
 public:
  BEGIN_CLASS_MAP(DOMProcessingInstruction)
  RECURSIVE_PARENT_CLASS(DOMNode)
  END_CLASS_MAP(DOMProcessingInstruction)
  DECLARE_CLASS(DOMProcessingInstruction, DOMProcessingInstruction, DOMNode)

  // need to implement
  public: c_DOMProcessingInstruction();
  public: ~c_DOMProcessingInstruction();
  public: void t___construct(CStrRef name, CStrRef value = null_string);
  DECLARE_METHOD_INVOKE_HELPERS(__construct);
  public: Variant t___get(Variant name);
  DECLARE_METHOD_INVOKE_HELPERS(__get);
  public: Variant t___set(Variant name, Variant value);
  DECLARE_METHOD_INVOKE_HELPERS(__set);
  public: bool t___isset(Variant name);
  DECLARE_METHOD_INVOKE_HELPERS(__isset);
  public: Variant t___destruct();
  DECLARE_METHOD_INVOKE_HELPERS(__destruct);

  // implemented by HPHP
  public: c_DOMProcessingInstruction *create(String name, String value = null_string);
  public: void dynConstruct(CArrRef Params);
  public: void getConstructor(MethodCallPackage &mcp);
  public: virtual void destruct();

};

///////////////////////////////////////////////////////////////////////////////
// class DOMNodeIterator

FORWARD_DECLARE_CLASS_BUILTIN(DOMNodeIterator);
class c_DOMNodeIterator : public ExtObjectData, public Sweepable {
 public:
  BEGIN_CLASS_MAP(DOMNodeIterator)
  PARENT_CLASS(Iterator)
  END_CLASS_MAP(DOMNodeIterator)
  DECLARE_CLASS(DOMNodeIterator, DOMNodeIterator, ObjectData)

  // need to implement
  public: c_DOMNodeIterator();
  public: ~c_DOMNodeIterator();
  public: void t___construct();
  DECLARE_METHOD_INVOKE_HELPERS(__construct);
  public: Variant t_current();
  DECLARE_METHOD_INVOKE_HELPERS(current);
  public: Variant t_key();
  DECLARE_METHOD_INVOKE_HELPERS(key);
  public: Variant t_next();
  DECLARE_METHOD_INVOKE_HELPERS(next);
  public: Variant t_rewind();
  DECLARE_METHOD_INVOKE_HELPERS(rewind);
  public: Variant t_valid();
  DECLARE_METHOD_INVOKE_HELPERS(valid);
  public: Variant t___destruct();
  DECLARE_METHOD_INVOKE_HELPERS(__destruct);

  // implemented by HPHP
  public: c_DOMNodeIterator *create();
  public: void dynConstruct(CArrRef Params);
  public: void getConstructor(MethodCallPackage &mcp);
  public: virtual void destruct();

public:
  void reset_iterator(dom_iterable *objmap);

  dom_iterable *m_objmap;
  ArrayIter *m_iter;
  int m_index;
  Object m_curobj;
};

///////////////////////////////////////////////////////////////////////////////
// class DOMNamedNodeMap

FORWARD_DECLARE_CLASS_BUILTIN(DOMNamedNodeMap);
class c_DOMNamedNodeMap : public ExtObjectDataFlags<ObjectData::UseGet|ObjectData::UseSet>, public dom_iterable {
 public:
  BEGIN_CLASS_MAP(DOMNamedNodeMap)
  PARENT_CLASS(IteratorAggregate)
  END_CLASS_MAP(DOMNamedNodeMap)
  DECLARE_CLASS(DOMNamedNodeMap, DOMNamedNodeMap, ObjectData)

  // need to implement
  public: c_DOMNamedNodeMap();
  public: ~c_DOMNamedNodeMap();
  public: void t___construct();
  DECLARE_METHOD_INVOKE_HELPERS(__construct);
  public: Variant t_getnameditem(CStrRef name);
  DECLARE_METHOD_INVOKE_HELPERS(getnameditem);
  public: Variant t_getnameditemns(CStrRef namespaceuri, CStrRef localname);
  DECLARE_METHOD_INVOKE_HELPERS(getnameditemns);
  public: Variant t_item(int64 index);
  DECLARE_METHOD_INVOKE_HELPERS(item);
  public: Variant t___get(Variant name);
  DECLARE_METHOD_INVOKE_HELPERS(__get);
  public: Variant t___set(Variant name, Variant value);
  DECLARE_METHOD_INVOKE_HELPERS(__set);
  public: bool t___isset(Variant name);
  DECLARE_METHOD_INVOKE_HELPERS(__isset);
  public: Variant t_getiterator();
  DECLARE_METHOD_INVOKE_HELPERS(getiterator);
  public: Variant t___destruct();
  DECLARE_METHOD_INVOKE_HELPERS(__destruct);

  // implemented by HPHP
  public: c_DOMNamedNodeMap *create();
  public: void dynConstruct(CArrRef Params);
  public: void getConstructor(MethodCallPackage &mcp);
  public: virtual void destruct();

};

///////////////////////////////////////////////////////////////////////////////
// class DOMNodeList

FORWARD_DECLARE_CLASS_BUILTIN(DOMNodeList);
class c_DOMNodeList : public ExtObjectDataFlags<ObjectData::UseGet|ObjectData::UseSet>, public dom_iterable {
 public:
  BEGIN_CLASS_MAP(DOMNodeList)
  PARENT_CLASS(IteratorAggregate)
  END_CLASS_MAP(DOMNodeList)
  DECLARE_CLASS(DOMNodeList, DOMNodeList, ObjectData)

  // need to implement
  public: c_DOMNodeList();
  public: ~c_DOMNodeList();
  public: void t___construct();
  DECLARE_METHOD_INVOKE_HELPERS(__construct);
  public: Variant t_item(int64 index);
  DECLARE_METHOD_INVOKE_HELPERS(item);
  public: Variant t___get(Variant name);
  DECLARE_METHOD_INVOKE_HELPERS(__get);
  public: Variant t___set(Variant name, Variant value);
  DECLARE_METHOD_INVOKE_HELPERS(__set);
  public: bool t___isset(Variant name);
  DECLARE_METHOD_INVOKE_HELPERS(__isset);
  public: Variant t_getiterator();
  DECLARE_METHOD_INVOKE_HELPERS(getiterator);
  public: Variant t___destruct();
  DECLARE_METHOD_INVOKE_HELPERS(__destruct);

  // implemented by HPHP
  public: c_DOMNodeList *create();
  public: void dynConstruct(CArrRef Params);
  public: void getConstructor(MethodCallPackage &mcp);
  public: virtual void destruct();

};

///////////////////////////////////////////////////////////////////////////////
// class DOMException

FORWARD_DECLARE_CLASS_BUILTIN(DOMException);
class c_DOMException : public c_Exception {
 public:
  BEGIN_CLASS_MAP(DOMException)
  RECURSIVE_PARENT_CLASS(Exception)
  END_CLASS_MAP(DOMException)
  DECLARE_CLASS(DOMException, DOMException, Exception)

  // need to implement
  public: c_DOMException();
  public: ~c_DOMException();
  public: void t___construct(CStrRef message = "", int64 code = 0);
  DECLARE_METHOD_INVOKE_HELPERS(__construct);
  public: Variant t___destruct();
  DECLARE_METHOD_INVOKE_HELPERS(__destruct);

  // implemented by HPHP
  public: c_DOMException *create(String message = "", int64 code = 0);
  public: void dynConstruct(CArrRef Params);
  public: void getConstructor(MethodCallPackage &mcp);
  public: virtual void destruct();

};

///////////////////////////////////////////////////////////////////////////////
// class DOMImplementation

FORWARD_DECLARE_CLASS_BUILTIN(DOMImplementation);
class c_DOMImplementation : public ExtObjectData {
 public:
  BEGIN_CLASS_MAP(DOMImplementation)
  END_CLASS_MAP(DOMImplementation)
  DECLARE_CLASS(DOMImplementation, DOMImplementation, ObjectData)

  // need to implement
  public: c_DOMImplementation();
  public: ~c_DOMImplementation();
  public: void t___construct();
  DECLARE_METHOD_INVOKE_HELPERS(__construct);
  public: Variant t_createdocument(CStrRef namespaceuri = null_string, CStrRef qualifiedname = null_string, CObjRef doctypeobj = null_object);
  DECLARE_METHOD_INVOKE_HELPERS(createdocument);
  public: Variant t_createdocumenttype(CStrRef qualifiedname = null_string, CStrRef publicid = null_string, CStrRef systemid = null_string);
  DECLARE_METHOD_INVOKE_HELPERS(createdocumenttype);
  public: bool t_hasfeature(CStrRef feature, CStrRef version);
  DECLARE_METHOD_INVOKE_HELPERS(hasfeature);
  public: Variant t___destruct();
  DECLARE_METHOD_INVOKE_HELPERS(__destruct);

  // implemented by HPHP
  public: c_DOMImplementation *create();
  public: void dynConstruct(CArrRef Params);
  public: void getConstructor(MethodCallPackage &mcp);
  public: virtual void destruct();

};

///////////////////////////////////////////////////////////////////////////////
// class DOMXPath

FORWARD_DECLARE_CLASS_BUILTIN(DOMXPath);
class c_DOMXPath : public ExtObjectDataFlags<ObjectData::UseGet|ObjectData::UseSet>, public Sweepable {
 public:
  BEGIN_CLASS_MAP(DOMXPath)
  END_CLASS_MAP(DOMXPath)
  DECLARE_CLASS(DOMXPath, DOMXPath, ObjectData)

  // need to implement
  public: c_DOMXPath();
  public: ~c_DOMXPath();
  public: void t___construct(CVarRef doc);
  DECLARE_METHOD_INVOKE_HELPERS(__construct);
  public: Variant t_evaluate(CStrRef expr, CObjRef context = null_object);
  DECLARE_METHOD_INVOKE_HELPERS(evaluate);
  public: Variant t_query(CStrRef expr, CObjRef context = null_object);
  DECLARE_METHOD_INVOKE_HELPERS(query);
  public: bool t_registernamespace(CStrRef prefix, CStrRef uri);
  DECLARE_METHOD_INVOKE_HELPERS(registernamespace);
  public: Variant t_registerphpfunctions(CVarRef funcs = null);
  DECLARE_METHOD_INVOKE_HELPERS(registerphpfunctions);
  public: Variant t___get(Variant name);
  DECLARE_METHOD_INVOKE_HELPERS(__get);
  public: Variant t___set(Variant name, Variant value);
  DECLARE_METHOD_INVOKE_HELPERS(__set);
  public: bool t___isset(Variant name);
  DECLARE_METHOD_INVOKE_HELPERS(__isset);
  public: Variant t___destruct();
  DECLARE_METHOD_INVOKE_HELPERS(__destruct);

  // implemented by HPHP
  public: c_DOMXPath *create(Variant doc);
  public: void dynConstruct(CArrRef Params);
  public: void getConstructor(MethodCallPackage &mcp);
  public: virtual void destruct();

 public:
  xmlNodePtr m_node;
  p_DOMDocument m_doc;
  Array m_node_list;
  int m_registerPhpFunctions;
  Array m_registered_phpfunctions;
};

///////////////////////////////////////////////////////////////////////////////
}

#endif // __EXT_DOMDOCUMENT_H__
