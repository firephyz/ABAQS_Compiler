#include "rule.h"
#include "ast.h"

#include <string>
#include <memory>

namespace abaqs {
  CompilerRule::CompilerRule(const RuleType type,
                             const std::string&& name,
                             AST math)
  : type {type},
    var_name {name},
    math {std::move(math)} {}
}
