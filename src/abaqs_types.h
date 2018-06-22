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

  //TODO Revise comment
  // When compiler function is used by the RuleProcessor to parse the
  // rule math defintion, we will simply 'splice' the ASTNode (containing
  // the entire math definition of a given function) into the
  // Rule ASTNode. (Can't actually do direct splicing though since
  // the AST nodes are const. We will pretend like we are.)
  class CompilerFunction {
  public:
    CompilerFunction(const libsbml::FunctionDefinition& func);
    bool operator==(const CompilerFunction& func);

    std::string var_name;
    AST ast;

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
