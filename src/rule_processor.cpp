#include "rule_processor.h"
#include "compiler.h"
#include "ast.h"

#include "sbml/SBMLTypes.h"

#include <string>
#include <iostream>

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

    AST * math {convertMathToAST(rule.getMath())};

    compiler.rules.push_back(
      CompilerRule(type,
                   variable,
                   math));
  }

  AST *
  RuleProcessor::convertMathToAST(const libsbml::ASTNode * rule)
  {
    return nullptr;
  }
}
