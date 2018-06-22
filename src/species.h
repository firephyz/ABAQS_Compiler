#ifndef SPECIES_INCLUDED

#define SPECIES_INCLUDED

#include <sbml/SBMLTypes.h>

#include <string>
#include <vector>

namespace abaqs {
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
}

#endif