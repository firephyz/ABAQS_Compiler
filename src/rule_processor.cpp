#include "rule_processor.h"
#include "compiler.h"

#include "sbml/SBMLTypes.h"

//TODO: Remove iostream
#include <iostream>

namespace abaqs {

  RuleProcessor::RuleProcessor(Compiler& compiler)
  : compiler {compiler}
  {}

  void
  RuleProcessor::processRule(const libsbml::Rule& rule)
  {
    std::cout << "Processing " << rule.getId() << std::endl;

    for(CompilerFunction func : compiler.functions) {
      std::cout << "Function: " << func.name << std::endl;
    }
  }
}
