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

#include <compiler/statement/function_statement.h>
#include <compiler/analysis/analysis_result.h>
#include <compiler/expression/expression_list.h>
#include <compiler/analysis/file_scope.h>
#include <compiler/analysis/function_scope.h>
#include <compiler/statement/statement_list.h>
#include <util/util.h>
#include <compiler/expression/parameter_expression.h>
#include <compiler/expression/modifier_expression.h>
#include <compiler/option.h>
#include <compiler/analysis/dependency_graph.h>
#include <compiler/analysis/variable_table.h>
#include <compiler/analysis/class_scope.h>
#include <sstream>

using namespace HPHP;
using namespace std;
using namespace boost;

///////////////////////////////////////////////////////////////////////////////
// constructors/destructors

FunctionStatement::FunctionStatement
(STATEMENT_CONSTRUCTOR_PARAMETERS,
 bool ref, const std::string &name, ExpressionListPtr params,
 StatementListPtr stmt, int attr, const std::string &docComment)
  : MethodStatement(STATEMENT_CONSTRUCTOR_PARAMETER_VALUES,
                    ModifierExpressionPtr(), ref, name, params, stmt, attr,
                    docComment, false), m_ignored(false) {
}

StatementPtr FunctionStatement::clone() {
  FunctionStatementPtr stmt(new FunctionStatement(*this));
  stmt->m_stmt = Clone(m_stmt);
  stmt->m_params = Clone(m_params);
  stmt->m_modifiers = Clone(m_modifiers);
  return stmt;
}

///////////////////////////////////////////////////////////////////////////////
// parser functions

void FunctionStatement::onParse(AnalysisResultPtr ar, BlockScopePtr scope) {
  // note it's important to add to file scope, not a pushed FunctionContainer,
  // as a function may be declared inside a class's method, yet this function
  // is a global function, not a class method.
  FileScopePtr fileScope = dynamic_pointer_cast<FileScope>(scope);
  if (!fileScope->addFunction(ar, onInitialParse(ar, fileScope, false))) {
    m_ignored = true;
    return;
  }
  ar->recordFunctionSource(m_name, m_loc, fileScope->getName());
}

///////////////////////////////////////////////////////////////////////////////
// static analysis functions

std::string FunctionStatement::getName() const {
  return string("Function ") + m_funcScope.lock()->getName();
}

void FunctionStatement::analyzeProgramImpl(AnalysisResultPtr ar) {
  // registering myself as a parent in dependency graph, so that
  // (1) we can tell orphaned parents
  // (2) overwrite non-master copy of function declarations
  if (ar->isFirstPass()) {
    if (m_loc) {
      ar->getDependencyGraph()->addParent(DependencyGraph::KindOfFunctionCall,
                                          "", m_name, shared_from_this());
    } // else it's pseudoMain or artificial functions we added
  }
  FunctionScopePtr func =
    Construct::getFunctionScope(); // containing function scope
  FunctionScopePtr fs = getFunctionScope();
  // redeclared functions are automatically volatile
  if (func && fs->isVolatile()) {
    func->getVariables()->setAttribute(VariableTable::NeedGlobalPointer);
  }
  MethodStatement::analyzeProgramImpl(ar);
}

void FunctionStatement::inferTypes(AnalysisResultPtr ar) {
  MethodStatement::inferTypes(ar);
}

///////////////////////////////////////////////////////////////////////////////
// code generation functions

void FunctionStatement::outputPHP(CodeGenerator &cg, AnalysisResultPtr ar) {
  FunctionScopePtr funcScope = m_funcScope.lock();
  if (!funcScope->isUserFunction()) return;

  cg_printf("function ");
  if (m_ref) cg_printf("&");
  cg_printf("%s(", m_name.c_str());
  if (m_params) m_params->outputPHP(cg, ar);
  cg_indentBegin(") {\n");

  m_funcScope.lock()->outputPHP(cg, ar);
  if (m_stmt) m_stmt->outputPHP(cg, ar);
  cg_indentEnd("}\n");
}

bool FunctionStatement::hasImpl() const {
  return m_funcScope.lock()->isVolatile();
}

void FunctionStatement::outputCPPImpl(CodeGenerator &cg,
                                      AnalysisResultPtr ar) {
  CodeGenerator::Context context = cg.getContext();

  FunctionScopePtr funcScope = m_funcScope.lock();
  string fname = funcScope->getId(cg).c_str();
  bool pseudoMain = funcScope->inPseudoMain();
  string origFuncName = !pseudoMain ? funcScope->getOriginalName() :
          ("run_init::" + funcScope->getContainingFile()->getName());
  string funcSection;

  if (outputFFI(cg, ar)) return;

  if (context == CodeGenerator::NoContext) {
    string rname = cg.formatLabel(m_name);
    if (funcScope->isRedeclaring()) {
      cg.printf("g->GCI(%s) = &%s%s;\n", m_name.c_str(),
                Option::CallInfoPrefix, fname.c_str());
    }
    if (funcScope->isVolatile()) {
      cg_printf("g->declareFunctionLit(");
      cg_printString(m_name, ar, shared_from_this());
      cg_printf(");\n");
      cg_printf("g->FVF(%s) = true;\n", rname.c_str());
    }
    return;
  }

  if (context == CodeGenerator::CppDeclaration &&
      !funcScope->isInlined()) return;

  if (context == CodeGenerator::CppPseudoMain &&
      !pseudoMain) return;
  if (context == CodeGenerator::CppImplementation &&
      (funcScope->isInlined() || pseudoMain)) return;

  cg.setPHPLineNo(-1);

  if (pseudoMain && !Option::GenerateCPPMain) {
    if (context == CodeGenerator::CppPseudoMain) {
      if (cg.getOutput() != CodeGenerator::SystemCPP) {
        cg.setContext(CodeGenerator::NoContext); // no inner functions/classes
        funcScope->getVariables()->setAttribute(VariableTable::ForceGlobal);
        outputCPPStmt(cg, ar);
        funcScope->getVariables()->clearAttribute(VariableTable::ForceGlobal);
        cg.setContext(CodeGenerator::CppPseudoMain);
        return;
      }
    } else if (context == CodeGenerator::CppForwardDeclaration &&
               cg.getOutput() != CodeGenerator::SystemCPP) {
      return;
    }
  }

  if (context == CodeGenerator::CppImplementation) {
    printSource(cg);
  } else if (context == CodeGenerator::CppForwardDeclaration &&
             Option::GenerateCppLibCode) {
    cg_printf("\n");
    printSource(cg);
    cg.printDocComment(funcScope->getDocComment());
  }

  if (funcScope->isInlined()) cg_printf("inline ");

  TypePtr type = funcScope->getReturnType();
  if (type) {
    type->outputCPPDecl(cg, ar);
  } else {
    cg_printf("void");
  }

  funcSection = Option::FunctionSections[origFuncName];
  if (!funcSection.empty()) {
    cg_printf(" __attribute__ ((section (\".text.%s\")))",
              funcSection.c_str());
  }

  if (pseudoMain) {
    cg_printf(" %s%s(", Option::PseudoMainPrefix,
              funcScope->getContainingFile()->pseudoMainName().c_str());
  } else {
    cg_printf(" %s%s(", Option::FunctionPrefix, fname.c_str());
  }

  switch (context) {
  case CodeGenerator::CppForwardDeclaration:
    funcScope->outputCPPParamsDecl(cg, ar, m_params, true);
    cg_printf(");\n");
    if (funcScope->hasDirectInvoke()) {
      cg_printf("Variant %s%s(void *extra, CArrRef params);\n",
                Option::InvokePrefix, fname.c_str());
    }
    break;
  case CodeGenerator::CppDeclaration:
  case CodeGenerator::CppImplementation:
  case CodeGenerator::CppPseudoMain:
    {
      funcScope->outputCPPParamsDecl(cg, ar, m_params, false);
      cg_indentBegin(") {\n");
      const char *sys =
        (cg.getOutput() == CodeGenerator::SystemCPP ? "_BUILTIN" : "");
      if (pseudoMain) {
        cg_printf("PSEUDOMAIN_INJECTION%s(%s, %s%s);\n",
                  sys, origFuncName.c_str(), Option::PseudoMainPrefix,
                  funcScope->getContainingFile()->pseudoMainName().c_str());
      } else {
        if (m_stmt->hasBody()) {
          cg_printf("FUNCTION_INJECTION%s(%s);\n", sys, origFuncName.c_str());
        }
        outputCPPArgInjections(cg, ar, origFuncName.c_str(), ClassScopePtr(),
                               funcScope);
      }
      funcScope->outputCPP(cg, ar);
      cg.setContext(CodeGenerator::NoContext); // no inner functions/classes
      outputCPPStmt(cg, ar);
      cg.setContext(context);
      cg_indentEnd("} /* function */\n");
    }
    break;
  default:
    ASSERT(false);
  }
}
