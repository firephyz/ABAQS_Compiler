/* Various header dependency issues required me to separate
   this general AST header from the more specific ast.h
   header.
*/
#ifndef AST_BASE_INCLUDED

#define AST_BASE_INCLUDED

#include <sbml/SBMLTypes.h>

#include <memory>
#include <string>
#include <ostream>

namespace abaqs {

  class ASTNode {
  public:
    const ASTType type;
    
    ASTNode();
    ASTNode(const ASTType type);
    ASTNode(const ASTNode& tree) = delete;
    ASTNode(ASTNode&& tree) = default;

    virtual std::string to_string() const;

    friend std::ostream& operator<<(
      std::ostream& out, const ASTNode& tree);
  };

  class AST {
  public:
    std::unique_ptr<ASTNode> node;

    AST(const libsbml::ASTNode * node);
    AST(AST&& tree);

    friend std::ostream& operator<<(
      std::ostream& out, const AST& tree);
  };

  std::ostream& operator<<(std::ostream& out, const AST& tree);
  std::ostream& operator<<(std::ostream& out, const ASTNode& tree);
  ASTNode * convertSBMLToAST(const libsbml::ASTNode * rule);
}

#endif