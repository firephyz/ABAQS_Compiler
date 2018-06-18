#ifndef RULE_PROCESSOR_INCLUDED
#define RULE_PROCESSOR_INCLUDED

#include "abaqs_types.h"

namespace abaqs {
  // Forware declare for when compiler.h includes this file
  class Compiler;

  class RuleProcessor {
  private:
    Compiler& compiler;
//    std::vector<Graph>

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
