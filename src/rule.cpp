#include "rule.h"
#include "ast.h"

#include <string>
#include <memory>

namespace abaqs {
  CompilerRule::CompilerRule(const RuleType type,
                             const std::string& name,
                             const AST * math0)
  : type {type},
    var_name {std::move(name)},
    math (math0) {}
}
