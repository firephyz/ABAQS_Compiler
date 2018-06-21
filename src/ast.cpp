#include "ast.h"
#include "abaqs_types.h"

#include <ostream>
#include <string>

namespace abaqs {

  AST::AST()
    : type(ASTType::None)
  {}

  AST::AST(const ASTType type)
    : type(type)
  {}

  std::ostream&
  operator<<(std::ostream& out, const AST& tree)
  {
    out << tree.to_string();
    return out;
  }

  std::string
  AST::to_string() const
  {
    return "";
  }

  ASTFunction::ASTFunction(
    const std::string& name,
    const ASTType type)
    : AST(type)
    , name(name)
  {}

  ASTBuiltinFunction::ASTBuiltinFunction(
    const std::string& name,
    const ASTBuiltinType func)
    : ASTFunction(name, ASTType::BuiltinFunction)
    , func(func)
  {}

  std::string
  ASTBuiltinFunction::to_string() const
  {
    std::string result("(");
    result += name;
    for(auto child : children) {
      result += " " + child->to_string();
    }
    result += ")";

    return result;
  }

  ASTUserFunction::ASTUserFunction(const std::string& name)
    : ASTFunction(name, ASTType::UserFunction)
    , func(nullptr)
  {
    //TODO: Locate the CompilerFunction
  }

  std::string
  ASTUserFunction::to_string() const
  {
    std::string result("(");
    result += name;
    for(auto child : children) {
      result += " " + child->to_string();
    }
    result += ")";
    return result;
  }

  // std::ostream&
  // ASTUserFunction::operator<<(std::ostream& out)
  // {
  //   out << "(" << name;
  //   for(AST& tree : children) {
  //     out << " " << tree;
  //   }
  //   out << ")";
  //   return out;
  // }
  
  ASTParameter::ASTParameter(const std::string& name)
    : AST(ASTType::Parameter)
    , name(name)
    , param(nullptr)
  {}

  std::string
  ASTParameter::to_string() const
  {
    return name;
  }

  ASTNumber::ASTNumber(const double value)
    : AST(ASTType::Number)
    , value(value)
  {}

  std::string
  ASTNumber::to_string() const
  {
    return std::to_string(value);
  }
}