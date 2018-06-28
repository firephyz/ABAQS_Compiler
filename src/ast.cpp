#include "ast.h"
#include "abaqs_types.h"
#include "doc_checks.h"

#include <sbml/SBMLTypes.h>

#include <ostream>
#include <string>

namespace abaqs {

  AST::AST(const libsbml::ASTNode * node)
    : node(std::unique_ptr<ASTNode>(
        convertSBMLToAST(node)))
  {}

  AST::AST(AST&& tree)
    : node(std::move(tree.node))
  {}

  std::ostream&
  operator<<(std::ostream& out, const AST& tree)
  {
    out << *tree.node;
    return out;
  }

  ASTNode::ASTNode()
    : type(ASTType::None)
  {}

  ASTNode::ASTNode(const ASTType type)
    : type(type)
  {}

  std::ostream&
  operator<<(std::ostream& out, const ASTNode& tree)
  {
    out << tree.to_string();
    return out;
  }

  std::string
  ASTNode::to_string() const
  {
    return "";
  }

  ASTFunction::ASTFunction(
    const std::string& name,
    const ASTType type)
    : ASTNode(type)
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
  
  ASTParameter::ASTParameter(const std::string& name)
    : ASTNode(ASTType::Parameter)
    , name(name)
    , param(nullptr)
  {}

  std::string
  ASTParameter::to_string() const
  {
    return name;
  }

  ASTNumber::ASTNumber(const double value)
    : ASTNode(ASTType::Number)
    , value(value)
  {}

  std::string
  ASTNumber::to_string() const
  {
    return std::to_string(value);
  }

  // Disregard the next block comment. Is no longer accurate
  // but it's still a question I need to ask.
  /* Ask someone more knowledgable in c++ about this.
     I would really like to return a temporary rvalue from this
     function so that the caller can move the contents out and store
     it somewhere nice. This though, requires us to put a std::move
     on every return from this function.
     Instead, we return a normal AST and let the caller call std::move. This relies on the compiler performing RVO though.
  */
  // Only meant to be called in constructor init
  ASTNode *
  convertSBMLToAST(const libsbml::ASTNode * rule)
  {
    if(rule->isPiecewise()) {
      // for(uint i = 0; i < rule->getNumChildren(); ++i) {
      //   std::cout << libsbml::SBML_formulaToString(rule->getChild(i)) << std::endl;
      // }
      throw InvalidABAQSDocument(
        "Piecewise functions not currently supported.");
    }
    else if(rule->isFunction()) {
      ASTUserFunction * func = new ASTUserFunction(rule->getName());
      
      for(uint i = 0; i < rule->getNumChildren(); ++i) {
        func->children.push_back(convertSBMLToAST(rule->getChild(i)));
      }

      return func;
    }
    else if(rule->isOperator() ||
            rule->isRelational() ||
            rule->isLogical()) {
      std::string name;

      if(rule->isRelational()) {
        name += rule->getName();
      }
      else {
        name += rule->getOperatorName();
      }

      struct builtin_info_t info
        {determineBuiltinInfo(rule->getType())};
      ASTBuiltinFunction * op =
        new ASTBuiltinFunction(info.symbol, info.type);

      for(uint i = 0; i < rule->getNumChildren(); ++i) {
        op->children.push_back(convertSBMLToAST(rule->getChild(i)));
      }

      return op;
    }
    else if(rule->isName()) {
      return new ASTParameter(rule->getName());
    }
    else if(rule->isNumber()) {
      return new ASTNumber(rule->getValue());
    }
    else if(rule->isLambda()) {
      throw InvalidABAQSDocument("Lambda's not currently supported.");
    }
    else {
      throw InvalidABAQSDocument(
        "Invalid math node: " + 
        std::string(libsbml::SBML_formulaToString(rule)));
    }

    // Should never get here
    return new ASTNode(ASTType::None);
  }

  // Finds the symbol and AST type associated with this given
  // ASTNodeType_t.
  const struct builtin_info_t
  determineBuiltinInfo(const libsbml::ASTNodeType_t type)
  {
    switch(type) {
      case libsbml::ASTNodeType_t::AST_PLUS:
        return {"+", ASTBuiltinType::plus};
      case libsbml::ASTNodeType_t::AST_MINUS:
        return {"-", ASTBuiltinType::minus};
      case libsbml::ASTNodeType_t::AST_TIMES:
        return {"*", ASTBuiltinType::times};
      case libsbml::ASTNodeType_t::AST_DIVIDE:
        return {"/", ASTBuiltinType::divide};
      case libsbml::ASTNodeType_t::AST_FUNCTION_LOG:
        throw InvalidABAQSDocument(
          "libsbml math type \'log\' not currently supported.");
        //return {"log", ASTBuiltinType::log};
      case libsbml::ASTNodeType_t::AST_RELATIONAL_EQ:
        return {"==", ASTBuiltinType::eq};
      case libsbml::ASTNodeType_t::AST_RELATIONAL_GT:
        return {">", ASTBuiltinType::gt};
      case libsbml::ASTNodeType_t::AST_RELATIONAL_LT:
        return {"<", ASTBuiltinType::lt};
      case libsbml::ASTNodeType_t::AST_RELATIONAL_GEQ:
        return {">=", ASTBuiltinType::geq};
      case libsbml::ASTNodeType_t::AST_RELATIONAL_LEQ:
        return {"<=", ASTBuiltinType::leq};
      default:
        throw InvalidABAQSDocument(
          "Unknown libsbml math type: " + std::to_string(type));
    }
  }
}