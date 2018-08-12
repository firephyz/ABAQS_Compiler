#ifndef INTER_REP_INCLUDED
#define INTER_REP_INCLUDED

#include "ast.h"
#include "compiler.h"

#include <string>
#include <vector>
#include <utility>

namespace abaqs {

  enum class IRStatementType {
    ConstantAssignment,
    BinaryFunction
  };

  class IRSource {
  public:
    IRSource(const std::string& data);
    
    bool was_modified = false;
    std::string data;
  };

  class IRStatement {
  public:
    std::string target;
    IRSource src0;
    IRSource src1;
    std::string operation;
    IRStatementType type;
    bool is_valid = true;
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
    void stripUselessZeros();
    int findTargetStatement(int target_number);
    void constantFold(int statement_index);
    double lookupVariableBinding(const std::string& var);
    void storeVariableBindings(
      std::vector<std::string> vars,
      std::vector<ASTNode *> values);
  };
}

#endif