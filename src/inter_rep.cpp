#include "inter_rep.h"
#include "ast.h"
#include "compiler.h"

#include <string>

namespace abaqs {

  IRStatement::IRStatement(
    const std::string& target,
    const std::string& src0,
    const std::string& src1,
    const std::string& op)
    : target {target}
    , src0 {src0}
    , src1 {src1}
    , operation {op}
  {}

  const std::string
  IRStatement::toString() const
  {
    std::string result;
    result += target;
    result += " = ";
    result += src0;
    result += " " + operation + " ";
    result += src1;
    result += ";";
    return std::move(result);
  }

  InterRep::InterRep(const Compiler& compiler, const AST& tree)
    : statements()
    , last_temp(convertTree(tree.node.get()))
  {}

  int InterRep::temp_id = 0;

  std::string
  InterRep::convertTree(const ASTNode * node)
  {
    std::string target {"t" + std::to_string(temp_id)};
    ++temp_id;

    if(node->type == ASTType::Number ||
       node->type == ASTType::Parameter) {
      statements.emplace_back(
        IRStatement(
          target,
          node->to_string(),
          "0",
          "+"));
    }
    else if(node->type == ASTType::BuiltinFunction) {
      const ASTBuiltinFunction& func =
        dynamic_cast<const ASTBuiltinFunction&>(*node);
      std::string src0 = convertTree(func.children[0]);
      std::string src1 = convertTree(func.children[1]);
      statements.emplace_back(
        IRStatement(
          target,
          src0,
          src1,
          func.name));
    }
    else {
      throw CompilerRuntimeError("AST convert error.");
    }

    return std::move(target);
  }
}