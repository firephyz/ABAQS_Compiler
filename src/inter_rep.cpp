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
    : compiler {compiler}
    , bindings()
    , statements()
    , last_temp(convertTree(
        tree.node.get(), 
        IRConvertType::General))
  {}

  int InterRep::temp_id = 0;

  std::string
  InterRep::convertTree(
    const ASTNode * node, IRConvertType ctype)
  {
    std::string target {"t" + std::to_string(temp_id)};
    ++temp_id;

    if(node->type == ASTType::Number) {
      statements.emplace_back(
        IRStatement(
          target,
          node->to_string(),
          "0",
          "+"));
    }
    else if(node->type == ASTType::Parameter) {
      const ASTParameter& param =
        dynamic_cast<const ASTParameter&>(*node);
      switch(ctype) {
        case IRConvertType::General:
        statements.emplace_back(
          IRStatement(
            target,
            node->to_string(),
            "0",
            "+"));
          break;
        case IRConvertType::Lambda:
        statements.emplace_back(
          IRStatement(
            target,
            std::to_string(lookupVariableBinding(param.name)),
            "0",
            "+"));
        break;
      }
    }
    else if(node->type == ASTType::BuiltinFunction) {
      const ASTBuiltinFunction& func =
        dynamic_cast<const ASTBuiltinFunction&>(*node);
      std::string src0 = convertTree(
        func.children[0], ctype);
      std::string src1 = convertTree(
        func.children[1], ctype);
      statements.emplace_back(
        IRStatement(
          target,
          src0,
          src1,
          func.name));
    }
    else if(node->type == ASTType::UserFunction) {
      const ASTUserFunction& userfunc =
        dynamic_cast<const ASTUserFunction&>(*node);
      const CompilerFunction& func = *userfunc.func;
      try {
        storeVariableBindings(func.args, userfunc.children);
      }
      catch(CompilerRuntimeError& error) {
        if(error.what() == std::string("args")) {
          throw CompilerRuntimeError(
            "In a call to function \'" +
            userfunc.name +
            "\', the number of arguments given does not match "
            "the number declared in the function definition.");
        }

        throw error;
      }
      std::string src = convertTree(
        func.body.node.get(), IRConvertType::Lambda);
      statements.emplace_back(
        IRStatement(
          target,
          src,
          "0",
          "+"));
    }
    else {
      throw CompilerRuntimeError("AST convert error.");
    }

    return std::move(target);
  }

  void
  InterRep::storeVariableBindings(
    std::vector<std::string> vars,
    std::vector<ASTNode *> values)
  {
    // We don't need bindings from previous lambda forms
    bindings.clear();

    // convertTree handles the error
    if(vars.size() != values.size()) {
      throw CompilerRuntimeError("args");
    }

    for(uint i = 0; i < vars.size(); ++i) {
      std::string& var = vars[i];
      ASTNode * node = values[i];
      if(node->type != ASTType::Number) {
        throw CompilerRuntimeError(
          "Value for variable \'" +
          var +
          "\' is not a double.");
      }
      ASTNumber * num =
        dynamic_cast<ASTNumber *>(node);
      bindings.emplace_back(
        std::pair<std::string, double>(
          var, num->value));
    }
  }

  double
  InterRep::lookupVariableBinding(const std::string& var)
  {
    for(auto pair : bindings) {
      if(pair.first == var) {
        return pair.second;
      }
    }

    // Didn't find the variable reference in the lambda environment.
    // Look to see if we are refering to another function.
    const CompilerFunction * func =
      compiler.lookupFunctionName(var);

    if(func == nullptr) {
      throw CompilerRuntimeError(var);
    }
    else {
      throw CompilerRuntimeError("Function names are not supported.");
    }

    return 0;
  }
}