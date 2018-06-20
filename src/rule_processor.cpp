#include "rule_processor.h"
#include "compiler.h"
#include "ast.h"
#include "doc_checks.h"

#include "sbml/SBMLTypes.h"

#include <string>
#include <memory>
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

    // compiler.rules.push_back(
    //   CompilerRule(type,
    //                std::move(variable),
    //                convertMathToAST(rule.getMath())));
  }

  AST&&
  RuleProcessor::convertMathToAST(const libsbml::ASTNode * rule)
  {
    // auto makeRoot = [rule&]()
    // {
    //   if(rule->isFunction()) {
    //     std::string name {rule->getName()};
    //     return ASTUserFunction(std::move(name));
    //   }
    //   else {
    //     ASTBuiltinType type {determineBuiltinType(rule->getType())};
    //   }
    // }

    // return std::move(root);

    return std::move(AST());
  }

  ASTBuiltinType
  determineBuiltinType(const libsbml::ASTNodeType_t type)
  {
    switch(type) {
      case libsbml::ASTNodeType_t::AST_PLUS:
        return ASTBuiltinType::plus;
      default:
        throw InvalidABAQSDocument(
          "Unknown libsbml math type: " + type);
    }
  }
}
