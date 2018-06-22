#ifndef ABAQS_TYPES_INCLUDED

#define ABAQS_TYPES_INCLUDED

// Don't include all of the compiler specfic ASTNodes.
// This will cause circular dependencies with
// CompilerFunction and others.
#include "ast_base.h"

#include <sbml/SBMLTypes.h>

#include <vector>
#include <memory>
#include <string>
#include <ostream>

namespace abaqs {

  class CompilerVar {
    //TODO Internal compiler variable. Not a parameter.
  };

  class CompilerSpecies {
  public:
    CompilerSpecies(const std::string name);
    bool operator==(const CompilerSpecies& sp);
    std::string name;

    enum class SpeciesType {
      cell,
      autoinducer
    };
  };

  class SpeciesRecord {
  public:
    void record(const libsbml::Species& sp);
    void storeCompilerSpecies(const std::string name,
      const CompilerSpecies::SpeciesType type);

    // Shouldn't have hundreds of cells and autoinducers.
    // Vectors should be plenty efficient.
    std::vector<CompilerSpecies> cells;
    std::vector<CompilerSpecies> autoinducers;
  };

  class CompilerParameter {
  public:
    CompilerParameter(const std::string name,
                      double value,
                      const bool is_constant);
    CompilerParameter(const std::string name,
                            const bool is_constant);
    // Equality for parameters only cares if the name matches
    bool operator==(const CompilerParameter& p);

    const std::string name;
    double value;
    const bool is_constant;
    bool value_is_valid;
  };

  class ParameterList : public std::vector<CompilerParameter> {
  public:
    void record(const libsbml::Parameter& p);
  };

  // When compiler function is used by the RuleProcessor to parse the
  // rule math defintion, we will simply 'splice' the ASTNode (containing
  // the entire math definition of a given function) into the
  // Rule ASTNode. (Can't actually do direct splicing though since
  // the AST nodes are const. We will pretend like we are.)
  class CompilerFunction {
  public:
    CompilerFunction(const libsbml::FunctionDefinition& func);
    bool operator==(const CompilerFunction& func);

    std::string var_name;
    AST ast;

    friend std::ostream& operator<<(std::ostream& out,
      const CompilerFunction& func);
  };

  std::ostream& operator<<(std::ostream& out,
      const CompilerFunction& func);

  class CompilerInitAssignment {
  public:
    std::string var_name;
    AST ast;

    CompilerInitAssignment(const libsbml::InitialAssignment& init);
  };

  class FunctionList : public std::vector<CompilerFunction> {
  public:
    void record(const libsbml::FunctionDefinition& func);
  };
}

#endif
