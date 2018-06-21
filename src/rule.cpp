#include "rule.h"
#include "ast.h"
#include "doc_checks.h"

#include <sbml/SBMLTypes.h>

#include <iostream>
#include <string>
#include <memory>

namespace abaqs {
  CompilerRule::CompilerRule(const RuleType type,
                             const std::string&& name,
                             const libsbml::ASTNode * math)
  : type {type},
    var_name {name},
    math {std::unique_ptr<AST>(
      convertSBMLToAST(math))}
  {}

  std::ostream&
  operator<<(std::ostream& out, const CompilerRule& rule)
  {
    out << "RULE: {\n\t" << rule.var_name
      << "\n\t" << *rule.math << "\n}\n";

    return out;
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
  AST *
  CompilerRule::convertSBMLToAST(const libsbml::ASTNode * rule)
  {
    if(rule->isFunction()) {
      ASTUserFunction * func = new ASTUserFunction(rule->getName());
      
      for(uint i = 0; i < rule->getNumChildren(); ++i) {
        func->children.push_back(convertSBMLToAST(rule->getChild(i)));
      }

      return func;
    }
    else if(rule->isOperator()) {
      ASTBuiltinFunction * op = new ASTBuiltinFunction(
        rule->getOperatorName(),
        determineBuiltinType(rule->getType()));

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
    else {
      throw InvalidABAQSDocument(
        "Invalid math node: " + 
        std::string(libsbml::SBML_formulaToString(rule)));
    }

    // Should never get here
    return new AST(ASTType::None);
  }

  ASTBuiltinType
  determineBuiltinType(const libsbml::ASTNodeType_t type)
  {
    switch(type) {
      case libsbml::ASTNodeType_t::AST_PLUS:
        return ASTBuiltinType::plus;
      case libsbml::ASTNodeType_t::AST_TIMES:
        return ASTBuiltinType::times;
      default:
        throw InvalidABAQSDocument(
          "Unknown libsbml math type: " + type);
    }
  }
}
