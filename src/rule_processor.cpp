#include "rule_processor.h"
#include "compiler.h"

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

    compiler.rules.push_back(
      CompilerRule(type,
                   std::move(variable),
                   rule.getMath()));
  }
}
