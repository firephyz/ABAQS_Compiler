#ifndef ABAQS_TYPES_INCLUDED

#define ABAQS_TYPES_INCLUDED

#include <sbml/SBMLTypes.h>

#include <vector>
#include <string>

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
}

#endif
