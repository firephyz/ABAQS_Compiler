#ifndef RULE_PROCESSOR_INCLUDED
#define RULE_PROCESSOR_INCLUDED

#include "ast.h"

#include <sbml/SBMLTypes.h>

namespace abaqs {
  // Forware declare for the reference used just below
  class Compiler;

  class RuleProcessor {
  private:
    Compiler& compiler;

  public:
      RuleProcessor(Compiler& compiler);
      void processRule(const libsbml::Rule& rule);
  };
}

// compiler.h needs to know the entire layout of RuleProcessor.
// So when rule_processor is included anywhere, the class definition
// must come first.
#include "compiler.h"

#endif
