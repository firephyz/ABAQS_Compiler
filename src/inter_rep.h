#ifndef INTER_REP_INCLUDED
#define INTER_REP_INCLUDED

#include "ast.h"
#include "compiler.h"

#include <string>
#include <vector>
#include <utility>

namespace abaqs {

  class IRStatement {
    std::string target;
    std::string src0;
    std::string src1;
    std::string operation;
  public:
    IRStatement(
      const std::string& target,
      const std::string& src0,
      const std::string& src1,
      const std::string& op);
    const std::string toString() const;

    friend class Compiler;
  };

  // Selects how variable names are resolved.
  enum class IRConvertType {
    // Searches parameters and function definitions
    General,
    // Searches lambda bindings and function definitions
    Lambda
  };

  using BindingFrame =
    std::vector<std::pair<std::string, double>>;

  class InterRep {
    static int temp_id;

    const Compiler& compiler;
    BindingFrame bindings;
  public:
    std::vector<IRStatement> statements;
    std::string last_temp;

    InterRep(const Compiler& compiler,
             const AST& tree,
             const IRConvertType type);

    // Binding frame pointer is a pointer to a frame constructed
    // above this lambda form (i.e. we were already inside
    // a lambda form so it constructed our bindings for us).
    std::string convertTree(
      const ASTNode * node,
      IRConvertType ctype);
    double lookupVariableBinding(
      const std::string& var,
      const BindingFrame* frame);
    void storeVariableBindings(
      std::vector<std::string> vars,
      std::vector<ASTNode *> values,
      const bool has_needed_bindings);
  };
}

#endif