#ifndef AST_INCLUDED
#define AST_INCLUDED

#include "abaqs_types.h"

#include <ostream>
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
    plus,
    times
  };

  class AST {
  public:
    const ASTType type;
    
    AST();
    AST(const ASTType type);
    AST(const AST& tree) = delete;
    AST(AST&& tree) = default;

    virtual std::string to_string() const;

    friend std::ostream& operator<<(
      std::ostream& out, const AST& tree);
  };

  std::ostream& operator<<(std::ostream& out, const AST& tree);

  class ASTNumber : public AST {
  public:
    const double value;

    ASTNumber(const double value);

    std::string to_string() const;
  };

  class ASTParameter : public AST {
  public:
    const std::string name;
    const CompilerParameter * param;

    ASTParameter(const std::string& name);

    std::string to_string() const;
  };

  class ASTFunction : public AST {
  public:
    const std::string name;
    std::vector<AST *> children;
    ASTFunction(const std::string& name,
      const ASTType type);
  };

  class ASTBuiltinFunction : public ASTFunction {
  public:
    ASTBuiltinType func;

    ASTBuiltinFunction(
      const std::string& name,
      const ASTBuiltinType func);

    std::string to_string() const;
  };

  class ASTUserFunction : public ASTFunction {
  public:
    CompilerFunction * func;

    ASTUserFunction(const std::string& name);
    ASTUserFunction(ASTUserFunction&& tree) = default;

    std::string to_string() const;
  };

  // class ASTApply : public AST {
  // private:
  //   const AST func;
  //   const std::vector<AST> args;
  // public:
  //   ASTApply(
  //     const AST func,
  //     std::vector<AST> args);
  //   ASTFunction& getFunction();
  //   std::vector<AST> getArgs();
  // };
}

#endif
