#include "rule_processor.h"

#include "sbml/SBMLTypes.h"

#include <iostream>

namespace abaqs {

  void
  RuleProcessor::initMemberVariables(struct RuleProcessorData& data)
  {
    species = data.species;
    parameters = data.parameters;
  }

  void
  RuleProcessor::processRule(const libsbml::Rule& rule)
  {
    std::cout << "Processing " << rule.getId() << std::endl;
  }
}
