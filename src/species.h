#ifndef SPECIES_INCLUDED

#define SPECIES_INCLUDED

#include <sbml/SBMLTypes.h>

#include <string>
#include <vector>

namespace abaqs {
  enum class SpeciesType {
    cell,
    autoinducer
  };

  class CompilerSpecies {
  public:
    CompilerSpecies(const std::string& name);
    bool operator==(const CompilerSpecies& sp);
    std::string name;
  };

  class SpeciesRecord {
  public:
    void storeCompilerSpecies(const std::string name);

    // Shouldn't have hundreds of cells and autoinducers.
    // Vectors should be plenty efficient.
    std::vector<CompilerSpecies> cells;
    std::vector<CompilerSpecies> autoinducers;

    bool speciesNameIsPresent(
      SpeciesType type, const std::string& name);
  };
}

#endif