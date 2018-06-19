#ifndef RULE_PROCESSOR_INCLUDED
#define RULE_PROCESSOR_INCLUDED

#include "abaqs_types.h"
#include "ast.h"

namespace abaqs {
  // Forware declare for when compiler.h includes this file
  class Compiler;

  class RuleProcessor {
  private:
    Compiler& compiler;

  public:
      RuleProcessor(Compiler& compiler);
      void processRule(const libsbml::Rule& rule);
      AST * convertMathToAST(const libsbml::ASTNode * rule);
  };
}

// compiler.h needs to know the entire layout of RuleProcessor.
// So when rule_processor is included anywhere, the class definition
// must come first.
#include "compiler.h"

#endif
