/* Internal representation of a SBML rule. Contains
 * information like the name of the rule and the AST
 * of the math information for the rule.
 */

#include "rule.h"
#include "ast.h"

#include <sbml/SBMLTypes.h>

#include <iostream>

namespace abaqs {

  // Constructs the AST when we initialize the CompilerRule
  CompilerRule::CompilerRule(const RuleType type,
                             std::string name,
                             const libsbml::ASTNode * sbml_math)
  : type {type}
  , var_name {std::move(name)}
  , math {AST(sbml_math)}
  {}

  std::ostream&
  operator<<(std::ostream& out, const CompilerRule& rule)
  {
    out << "RULE: {\n\t" << rule.var_name
      << "\n\t" << rule.math << "\n}\n";

    return out;
  }
}
