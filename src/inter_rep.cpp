// As per SBML spec 3.2, lambda forms inside
// functions cannot reference names outside of
// the lambda expression (there is not global namespace).
// Values must be passed into the function call.
// I am assuming also that you cannot have nested 
// lambda forms even inside a FunctionDefinition.
// This may be wrong, needs careful reading of the spec.
//
// I break these above specs only in that I allow
// other functions to be called in a lambda body even
// though they aren't passed as an argument. 
// This is handled by letting the current InterRep
// object A create another instance of itself B but with
// the other function as the target. We then append
// the intermediate rep of B to that of A.
//
// All this means for now is that the InterRep object
// needs only one context frame to resolve names.

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

  InterRep::InterRep(const Compiler& compiler,
                     const AST& tree,
                     const IRConvertType type)
    : compiler {compiler}
    , bindings()
    , statements()
    , last_temp(convertTree(
        tree.node.get(), 
        type))
  {}

  int InterRep::temp_id = 0;

  std::string
  InterRep::convertTree(
    const ASTNode * node,
    IRConvertType ctype)
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
            std::to_string(compiler.lookupParameterValue(param.name)),
            "0",
            "+"));
          break;
        case IRConvertType::Lambda:
        statements.emplace_back(
          IRStatement(
            target,
            std::to_string(
              lookupVariableBinding(param.name, &bindings)),
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
        bool has_needed_bindings = false;
        if(ctype == IRConvertType::Lambda) {
          has_needed_bindings = true;
        }
        storeVariableBindings(
          func.args,
          userfunc.children,
          has_needed_bindings);
        // If already in lambda form, recursively call
        // another instance of intermediate representation.
        // We will just copy the statements over when it's done.
        if(ctype == IRConvertType::Lambda) {

          const InterRep sub_ir(
            compiler,
            func.body,
            IRConvertType::Lambda);
          for(const IRStatement& statement : sub_ir.statements) {
            statements.emplace_back(statement);
          }
        }
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
    std::vector<ASTNode *> values,
    const bool has_needed_bindings)
  {
    // We don't need bindings from previous lambda forms
    // encountered during other function calls
    BindingFrame new_frame;
    BindingFrame* target_frame = nullptr;
    if(has_needed_bindings) {
      new_frame = bindings;
      target_frame = &new_frame;
    }
    else {
      target_frame = &bindings;
    }
    bindings.clear();

    // convertTree handles the error
    if(vars.size() != values.size()) {
      throw CompilerRuntimeError("args");
    }

    for(uint i = 0; i < vars.size(); ++i) {
      std::string& var = vars[i];
      ASTNode * node = values[i];
      double value = 0;
      if(node->type == ASTType::Parameter) {
        ASTParameter& param = static_cast<ASTParameter&>(*node);
        value = lookupVariableBinding(
          param.name, target_frame);
      }
      else if(node->type == ASTType::Number) {
        ASTNumber& num = static_cast<ASTNumber&>(*node);
        value = num.value;
      }
      else {
        throw CompilerRuntimeError(
          "Binding for variable \'" +
          var +
          "\' is not a number, variable or parameter.");
      }
      bindings.emplace_back(
        std::pair<std::string, double>(var, value));
    }
  }

  double
  InterRep::lookupVariableBinding(
    const std::string& var,
    const BindingFrame* frame)
  {
    for(auto pair : *frame) {
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