#include "rule_processor.h"
#include "compiler.h"
#include "ast.h"

#include "sbml/SBMLTypes.h"

namespace abaqs {

  RuleProcessor::RuleProcessor(Compiler& compiler)
  : compiler {compiler}
  {}

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

    compiler.rules.emplace_back(
      CompilerRule(type, variable, rule.getMath()));
  }
}
