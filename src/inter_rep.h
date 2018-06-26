#ifndef INTER_REP_INCLUDED
#define INTER_REP_INCLUDED

#include "ast.h"
#include "compiler.h"

#include <string>
#include <vector>

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

  class InterRep {
    static int temp_id;
  public:
    std::vector<IRStatement> statements;
    std::string last_temp;

    InterRep(const Compiler& compiler, const AST& tree);

    std::string convertTree(const ASTNode * node);
  };
}

#endif