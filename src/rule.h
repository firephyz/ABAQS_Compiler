#ifndef RULE_INCLUDED
#define RULE_INCLUDED

#include "ast.h"

#include <iostream>
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
    std::string var_name;
    AST math;
    
    CompilerRule(const RuleType type,
                 const std::string&& name,
                 const libsbml::ASTNode * math);
    CompilerRule(CompilerRule&& rule) = default;
    // Don't want to copy the AST
    CompilerRule(const CompilerRule& rule) = delete;

    friend std::ostream& operator<<(
      std::ostream& out, const CompilerRule& tree);
  };

  std::ostream& operator<<(
      std::ostream& out, const CompilerRule& tree);
}

#endif
