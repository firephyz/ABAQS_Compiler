// As per SBML spec 3.2, lambda forms inside
// functions cannot reference names outside of
// the lambda expression (there is not global namespace).
// Values must be passed into the function call.
// I am assuming also that you cannot have nested 
// lambda forms even inside a FunctionDefinition.
//
// All this means for now is that the InterRep object
// needs only one context frame to resolve names.

#include "inter_rep.h"
#include "ast.h"
#include "compiler.h"

#include <string>

namespace abaqs {

  IRSource::IRSource(const std::string& data)
    : type {IRSourceType::Node}
    , data {data}
  {
    if(data[0] != 't') {
      type = IRSourceType::Number;
    }
  }

  IRStatement::IRStatement(
    const std::string& target,
    const std::string& src0,
    const std::string& src1,
    const std::string& op)
    : target {target}
    , src0 {std::move(IRSource(src0))}
    , src1 {std::move(IRSource(src1))}
    , operation {determineOperationType(op)}
    , type {IRStatementType::BinaryFunction}
  {}

  const std::string
  IRStatement::toString() const
  {
    std::string result;
    result += target;
    result += " = ";
    result += src0.data;

    if(type == IRStatementType::BinaryFunction) {
      result += " " + IROpToString(operation) + " ";
      result += src1.data;
    }

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

  void
  InterRep::stripUselessZeros()
  {
    for(auto& statement : statements) {
      if(statement.src1.data == "0") {
        statement.type = IRStatementType::ConstantAssignment;
      }
    }
  }

  void
  performArithmeticOnConstantExpr(
    IRStatement& statement)
  {
    double arg0 = std::stod(statement.src0.data);
    double arg1 = std::stod(statement.src1.data);
    double result = 0;

    switch(statement.operation) {
      case IROperation::Plus:
        result = arg0 + arg1;
        break;
      case IROperation::Sub:
        result = arg0 - arg1;
        break;
      case IROperation::Mult:
        result = arg0 * arg1;
        break;
      case IROperation::Div:
        if(arg1 == 0) {
          throw CompilerRuntimeError(
            "Compiler attempting to constant fold a "
            "division by zero.");
        }
        result = arg0 / arg1;
        break;
    }

    statement.src0.data = std::to_string(result);
    statement.src0.was_modified = false;
    statement.type = IRStatementType::ConstantAssignment;
  }

  // TODO: The statement that replaces a source value
  // might itself be constant but not a number. We must loop and continue
  // to code fold down until all we have are numbers.
  void
  InterRep::constantFold(int statement_index)
  {
    auto& statement = statements[statement_index];
    if(statement.type != IRStatementType::BinaryFunction) {
      throw CompilerRuntimeError("Cannot code fold a constant.");
    }

    // Locate the correct statement to constant fold
    do {
      int child0_index = findTargetStatement(statement.src0);

      if(statements[child0_index].type == IRStatementType::BinaryFunction) {
        constantFold(child0_index);
      }
      else {
        statement.src0 = statements[child0_index].src0;
        statements[child0_index].is_valid = false;
        statement.src0.was_modified = true;
        if(statement.src0.type == IRSourceType::Number) {
          break;
        }
      }
    } while(true);

    do {
      int child1_index = findTargetStatement(statement.src1);

      if(statements[child1_index].type == IRStatementType::BinaryFunction) {
        constantFold(child1_index);
      }
      else {
        statement.src1 = statements[child1_index].src0;
        statements[child1_index].is_valid = false;
        statement.src1.was_modified = true;
        if(statement.src1.type == IRSourceType::Number) {
          break;
        }
      }
    } while(true);

    if(statement.src0.was_modified &&
       statement.src1.was_modified) {
      performArithmeticOnConstantExpr(statement);
    }
  }

  // TODO: Implemented search using inefficient linear search.
  // Do something else.
  int
  InterRep::findTargetStatement(const IRSource& src)
  {
    int target_number = std::stoi(src.data.substr(1));
    int counter = 0;
    for(auto& statement : statements) {
      if(std::stoi(statement.target.substr(1)) == target_number) {
        return counter;
      }

      ++counter;
    }

    throw CompilerRuntimeError(
      "Requesting a statement number that doesn't exist.");
  }

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
              lookupVariableBinding(param.name)),
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
        value = lookupVariableBinding(param.name);
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

  // Arithmetic operation lookup table
  using table_type =
    std::vector<std::pair<char, IROperation>>;
  const table_type op_table =
    {{'+', IROperation::Plus},
     {'-', IROperation::Sub},
     {'*', IROperation::Mult},
     {'/', IROperation::Div}};

  IROperation
  determineOperationType(const std::string& op)
  {
    for(auto& pair : op_table) {
      if(op[0] == pair.first) {
        return pair.second;
      }
    }

    throw CompilerRuntimeError(
      "Couldn't determine operation type for " + op);
  }

  std::string
  IROpToString(const IROperation& op)
  {
    for(auto& pair : op_table) {
      if(op == pair.second) {
        return std::to_string(pair.first);
      }
    }

    throw CompilerRuntimeError(
      "Internal compiler error: Missing op_table value "
      "for operation number " + static_cast<int>(op));
  }
}