#include "rule.h"
#include "ast.h"

#include <sbml/SBMLTypes.h>

#include <iostream>

namespace abaqs {
  CompilerRule::CompilerRule(const RuleType type,
                             const std::string&& name,
                             const libsbml::ASTNode * math)
  : type {type},
    var_name {name},
    math {AST(math)}
  {}

  std::ostream&
  operator<<(std::ostream& out, const CompilerRule& rule)
  {
    out << "RULE: {\n\t" << rule.var_name
      << "\n\t" << rule.math << "\n}\n";

    return out;
  }
}
