#include "rule_processor.h"
#include "compiler.h"
#include "ast.h"

#include "sbml/SBMLTypes.h"

namespace abaqs {

  RuleProcessor::RuleProcessor(Compiler& compiler)
  : compiler {compiler} {}

  void
  RuleProcessor::processRule(const libsbml::Rule& rule)
  {
    std::string variable = rule.getVariable();

    RuleType type {RuleType::assignment};
    if(!rule.isAssignment()) {
      if(rule.isRate()) {
        type = RuleType::rateRule;
      }
    }

    CompilerRule comp_rule(type, std::move(variable), rule.getMath());
    resolveFunctionNames(
      comp_rule.var_name,
      comp_rule.math.node.get());
    compiler.rules.push_back(std::move(comp_rule));
  }

  void
  RuleProcessor::resolveFunctionNames(
    const std::string& ruleName,
    ASTNode * node)
  {
    if(node->type == ASTType::BuiltinFunction ||
       node->type == ASTType::UserFunction) {      
      if(node->type == ASTType::UserFunction) {
        ASTUserFunction& userfunc =
          dynamic_cast<ASTUserFunction&>(*node);
        
        const CompilerFunction * cfunc {
          lookupCompilerFunction(userfunc.name)};

        if(cfunc == nullptr) {
          throw CompilerRuntimeError(
            "Compiler rule \'" +
            ruleName +
            "\' is using an undefined function "
            "definition \'" + userfunc.name + "\'.");
        }

        userfunc.func = cfunc;
      }

      ASTFunction& func =
        dynamic_cast<ASTFunction&>(*node);
      for(ASTNode * child : func.children) {
        resolveFunctionNames(ruleName, child);
      }
    }
  }

  const CompilerFunction *
  RuleProcessor::lookupCompilerFunction(const std::string& name)
  {
    for(const CompilerFunction& func : compiler.functions) {
      if(func.var_name == name) {
        return &func;
      }
    }
    
    return nullptr;
  }
}
