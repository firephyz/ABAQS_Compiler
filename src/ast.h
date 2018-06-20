#ifndef AST_INCLUDED
#define AST_INCLUDED

#include "abaqs_types.h"

#include <string>
#include <vector>

namespace abaqs {

  enum class ASTType {
    None,
    Number,
    Parameter,
    BuiltinFunction,
    UserFunction,
    Apply
  };

  enum class ASTBuiltinType {
    plus
  };

  class AST {
  public:
    const ASTType type;
    
    AST();
    AST(const ASTType type);
    AST(AST&& tree);
  };

  class ASTNumber : public AST {
  public:
    const double value;

    ASTNumber(const double value);
  };

  class ASTParameter : public AST {
  public:
    const std::string name;
    const CompilerParameter * param;

    ASTParameter(std::string&& name);
  };

  class ASTFunction : public AST {
  public:
    ASTBuiltinType func;

    ASTFunction(const ASTBuiltinType func);
  };

  class ASTUserFunction : public AST {
  public:
    const std::string name;
    CompilerFunction * func;

    ASTUserFunction(std::string&& name);
    ASTUserFunction(ASTUserFunction&& tree);
  };

  class ASTApply : public AST {
  private:
    const AST func;
    const std::vector<AST> args;
  public:
    ASTApply(
      const AST func,
      std::vector<AST> args);
    ASTFunction& getFunction();
    std::vector<AST> getArgs();
  };
}

#endif
