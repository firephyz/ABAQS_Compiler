#ifndef ABAQS_TYPES_INCLUDED

#define ABAQS_TYPES_INCLUDED

#include <sbml/SBMLTypes.h>

#include <vector>
#include <string>

namespace abaqs {

  class CompilerVar {
    static int id_counter;
    bool is_constant;
    // Value only valid if is_constant=true
    double value;

  public:
    CompilerVar(const char * var_name, double value); // Assume variable is constant
    CompilerVar(const char * var_name); // Variable is not constant

    const int id;
    const std::string name;
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

  class SpeciesList {
    // Shouldn't have hundreds of cells and autoinducers.
    // Vectors should be plenty efficient.
    std::vector<CompilerSpecies> cells;
    std::vector<CompilerSpecies> autoinducers;
  public:
    void record(const libsbml::Species& sp);
    void storeCompilerSpecies(const std::string name,
      const CompilerSpecies::SpeciesType type);
    std::vector<CompilerSpecies>& getCells();
    std::vector<CompilerSpecies>& getAutoinducers();
  };
}

#endif
