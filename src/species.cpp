#include "species.h"
#include "doc_checks.h"

#include <sbml/SBMLTypes.h>

#include <string>
#include <vector>

namespace abaqs {
  CompilerSpecies::CompilerSpecies(const std::string name)
  : name {std::move(name)}
  {}

  bool
  CompilerSpecies::operator==(const CompilerSpecies& sp)
  {
    return name == sp.name;
  }

  // TODO: Doesn't currently store species anywhere.
  // Do we need to? Given the specifics of the problem.
  void
  SpeciesRecord::record(const libsbml::Species& sp)
  {
    const std::string cell_prefix {"CELL_"};
    const std::string auto_prefix {"AUTO_"};
    std::string id = sp.getId();

    //TODO: Implement species cross referencing with the architecture
    // file so we know what type it is. Autoinducer or cell.
    // if(id.find(cell_prefix) != std::string::npos) {
    //   SpeciesRecord::storeCompilerSpecies(id,
    //     CompilerSpecies::SpeciesType::cell);
    // }
    // else if(id.find(auto_prefix) != std::string::npos) {
    //   SpeciesRecord::storeCompilerSpecies(id,
    //     CompilerSpecies::SpeciesType::autoinducer);
    // }
    // else {
    //   throw InvalidABAQSDocument(
    //     "Species \'" + id + "\' is missing the "
    //     "\'CELL_\' or \'AUTO_\' prefix."
    //   );
    // }
  }

  void
  SpeciesRecord::storeCompilerSpecies(const std::string name,
    const CompilerSpecies::SpeciesType type)
  {
    CompilerSpecies sp(name);

    switch(type) {
      case CompilerSpecies::SpeciesType::cell:
        if(std::find(cells.begin(), cells.end(), sp) != cells.end()) {
          throw InvalidABAQSDocument(
            "Reuse of species \'" + name + "\'."
          );
        }
        cells.push_back(std::move(sp));
        break;
      case CompilerSpecies::SpeciesType::autoinducer:
        if(std::find(autoinducers.begin(), autoinducers.end(), sp) != autoinducers.end()) {
          throw InvalidABAQSDocument(
            "Redeclaration of species \'" + name + "\'."
          );
        }
        autoinducers.push_back(std::move(sp));
        break;
    }
  }
}