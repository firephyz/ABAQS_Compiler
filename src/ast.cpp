#include "ast.h"
#include "abaqs_types.h"

#include <string>

namespace abaqs {

  AST::AST()
    : type(ASTType::None)
  {}

  AST::AST(const ASTType type)
    : type(type)
  {}

  ASTApply::ASTApply(
    AST func,
    std::vector<AST> args)
    : AST(ASTType::Apply)
    , func(std::move(func))
    , args(std::move(args))
  {}

  ASTFunction::ASTFunction(const ASTBuiltinType func)
    : AST(ASTType::BuiltinFunction)
    , func(func)
  {}

  ASTUserFunction::ASTUserFunction(std::string&& name)
    : AST(ASTType::UserFunction)
    , name(name)
    , func(nullptr)
  {
    //TODO: Locate the CompilerFunction
  }

  ASTUserFunction::ASTUserFunction(ASTUserFunction&& tree)
    : name(std::move(tree.name))
    , func(std::move(tree.func))
  {}
  
  ASTParameter::ASTParameter(std::string&& name)
    : AST(ASTType::Parameter)
    , name(name)
  {}

  ASTNumber::ASTNumber(const double value)
    : AST(ASTType::Number)
    , value(value)
  {}
}