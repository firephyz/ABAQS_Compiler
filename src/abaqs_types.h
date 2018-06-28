#ifndef ABAQS_TYPES_INCLUDED

#define ABAQS_TYPES_INCLUDED

// Don't include all of the compiler specfic ASTNodes.
// This will cause circular dependencies with
// CompilerFunction and others.
#include "ast_base.h"

// Include more specific types too
// ast_base.h needs to be included before this
#include "species.h"
#include "rule.h"

#include <sbml/SBMLTypes.h>

#include <vector>
#include <memory>
#include <string>
#include <ostream>

namespace abaqs {

  class CompilerParameter {
  public:
    CompilerParameter(const std::string name,
                      double value,
                      const bool is_constant);
    CompilerParameter(const std::string name,
                            const bool is_constant);
    CompilerParameter(CompilerParameter&& param);
    CompilerParameter();
    CompilerParameter& operator=(
      const CompilerParameter& param) = delete;
    CompilerParameter& operator=(
      CompilerParameter&& param) = default;
    // Check if names match
    bool operator==(const CompilerParameter& p);

    std::string name;
    double value;
    bool is_constant;
    bool value_is_valid;
  };

  class CompilerFunction {
    void recordFunctionArgs(const libsbml::ASTNode * node);
  public:
    CompilerFunction(const libsbml::FunctionDefinition& func);
    bool operator==(const CompilerFunction& func);

    std::string var_name;
    AST body;
    std::vector<std::string> args;

    friend std::ostream& operator<<(std::ostream& out,
      const CompilerFunction& func);
  };

  std::ostream& operator<<(std::ostream& out,
      const CompilerFunction& func);

  class CompilerInitAssignment {
  public:
    std::string var_name;
    AST ast;

    CompilerInitAssignment(const libsbml::InitialAssignment& init);
  };
}

#endif
