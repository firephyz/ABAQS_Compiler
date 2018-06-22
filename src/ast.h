#ifndef AST_INCLUDED
#define AST_INCLUDED

#include <sbml/SBMLTypes.h>

// Must include this little portion first so ast_base.h
// can know the ASTType type.
namespace abaqs {
    enum class ASTType {
    None,
    Number,
    Parameter,
    BuiltinFunction,
    UserFunction,
    Apply
  };
}

#include "ast_base.h"

#include "abaqs_types.h"

#include <ostream>
#include <string>
#include <vector>

namespace abaqs {

  enum class ASTBuiltinType {
    plus,
    times
  };

  class ASTNumber : public ASTNode {
  public:
    const double value;

    ASTNumber(const double value);

    std::string to_string() const;
  };

  class ASTParameter : public ASTNode {
  public:
    const std::string name;
    const CompilerParameter * param;

    ASTParameter(const std::string& name);

    std::string to_string() const;
  };

  class ASTFunction : public ASTNode {
  public:
    const std::string name;
    std::vector<ASTNode *> children;
    ASTFunction(const std::string& name,
      const ASTType type);
  };

  class ASTBuiltinFunction : public ASTFunction {
  public:
    ASTBuiltinType func;

    ASTBuiltinFunction(
      const std::string& name,
      const ASTBuiltinType func);

    std::string to_string() const;
  };

  class ASTUserFunction : public ASTFunction {
  public:
    CompilerFunction * func;

    ASTUserFunction(const std::string& name);
    ASTUserFunction(ASTUserFunction&& tree) = default;

    std::string to_string() const;
  };

  ASTNode * convertSBMLToAST(const libsbml::ASTNode * rule);
  // Determines ASTBuiltinType from libsbml type
  ASTBuiltinType determineBuiltinType(
    const libsbml::ASTNodeType_t type);
}

#endif
