/* Records species. Not very useful now but will be more useful
 * once we compile models with multiple different cell and chemical types.
 */

#include "species.h"
#include "doc_checks.h"

#include <sbml/SBMLTypes.h>

#include <string>
#include <vector>

namespace abaqs {
  CompilerSpecies::CompilerSpecies(const std::string& name)
  : name {name}
  {}

  bool
  CompilerSpecies::operator==(const CompilerSpecies& sp)
  {
    return name == sp.name;
  }

  void
  SpeciesRecord::storeCompilerSpecies(const std::string name)
  {
    //TODO: We don't have any use for this code as of now. Defaulting
    // species type to cell for simplicity.
    SpeciesType type = SpeciesType::cell;

    switch(type) {
      case SpeciesType::cell:
        if(speciesNameIsPresent(SpeciesType::cell, name)) {
          throw InvalidABAQSDocument(
            "Reuse of species \'" + name + "\'."
          );
        }
        cells.emplace_back(CompilerSpecies(name));
        break;
      case SpeciesType::autoinducer:
        if(speciesNameIsPresent(SpeciesType::autoinducer, name)) {
          throw InvalidABAQSDocument(
            "Redeclaration of species \'" + name + "\'."
          );
        }
        autoinducers.emplace_back(CompilerSpecies(name));
        break;
    }
  }

  // TODO: use of templates here can reduce the duplicate code.
  bool
  SpeciesRecord::speciesNameIsPresent(
    SpeciesType type, const std::string& name)
  {
    if(type == SpeciesType::cell) {
      for(auto& record : cells) {
        if(record.name == name) return true;
      }
    }
    else if(type == SpeciesType::autoinducer) {
      for(auto& record : autoinducers) {
        if(record.name == name) return true;
      }
    }

    return false;
  }
}