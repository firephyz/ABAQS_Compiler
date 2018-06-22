#include "abaqs_types.h"
#include "doc_checks.h"

#include <sbml/SBMLTypes.h>

#include <vector>
#include <string>
#include <memory>
#include <ostream>

namespace abaqs {

  CompilerParameter::CompilerParameter(const std::string name,
                                       double value,
                                       const bool is_constant)
    : name {std::move(name)}
    , value {value}
    , is_constant {is_constant}
    , value_is_valid {true}
  {}

  CompilerParameter::CompilerParameter(const std::string name,
                                       const bool is_constant)
    : name {std::move(name)}
    , is_constant {is_constant}
    , value_is_valid {false}
  {}

  CompilerParameter::CompilerParameter()
    : name("")
    , value(0)
    , is_constant(false)
    , value_is_valid(false)
  {}

  CompilerParameter::CompilerParameter(CompilerParameter&& param)
    : name(std::move(param.name))
    , value(param.value)
    , is_constant(param.is_constant)
    , value_is_valid(param.value_is_valid)
  {}

  bool
  CompilerParameter::operator==(const CompilerParameter& p)
  {
    return name == p.name;
  }

  CompilerFunction::CompilerFunction(const libsbml::FunctionDefinition& func)
  : var_name {func.getId()},
    ast {AST(func.getBody())}
  {}

  bool
  CompilerFunction::operator==(const CompilerFunction& func)
  {
    return var_name == func.var_name;
  }

  std::ostream&
  operator<<(std::ostream& out, const CompilerFunction& func)
  {
    out << "FUNCTION: {\n\t" << func.var_name
      << "\n\t" << func.ast << "\n}\n";

    return out;
  }

  CompilerInitAssignment::CompilerInitAssignment(
    const libsbml::InitialAssignment& init)
    : var_name(init.getSymbol())
    , ast(AST(init.getMath()))
  {}
}
