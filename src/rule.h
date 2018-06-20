#ifndef RULE_INCLUDED
#define RULE_INCLUDED

#include "ast.h"

#include <string>
#include <memory>

namespace abaqs {

  enum class RuleType {
    assignment,
    rateRule
  };

  class CompilerRule {
  public:
    const RuleType type;
    const std::string var_name;
    const AST math;
    
    CompilerRule(const RuleType type,
                 const std::string&& name,
                 AST math);
  };
}

#endif
