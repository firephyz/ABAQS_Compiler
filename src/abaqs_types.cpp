#include "abaqs_types.h"
#include "doc_checks.h"

#include <sbml/SBMLTypes.h>

#include <vector>
#include <string>
#include <utility>

namespace abaqs {

  int CompilerVar::id_counter = 0;

  CompilerVar::CompilerVar(const char * var_name)
  : name {var_name}, is_constant {false}, id {id_counter}
  {
    ++id_counter;
  }

  CompilerVar::CompilerVar(const char * var_name, double init_value)
  : CompilerVar(var_name)
  {
    is_constant = true;
  }

  CompilerSpecies::CompilerSpecies(const std::string name)
  : name {std::move(name)}
  {}

  bool
  CompilerSpecies::operator==(const CompilerSpecies& sp)
  {
    return name == sp.name;
  }

  void
  SpeciesList::record(const libsbml::Species& sp)
  {
    const std::string cell_prefix {"CELL_"};
    const std::string auto_prefix {"AUTO_"};
    std::string id = sp.getId();

    if(id.find(cell_prefix) != std::string::npos) {
      std::string name {id.substr(cell_prefix.length(), std::string::npos)};
      SpeciesList::storeCompilerSpecies(name,
        CompilerSpecies::SpeciesType::cell);
    }
    else if(id.find(auto_prefix) != std::string::npos) {
      std::string name {id.substr(auto_prefix.length(), std::string::npos)};
      SpeciesList::storeCompilerSpecies(name,
        CompilerSpecies::SpeciesType::autoinducer);
    }
    else {
      throw InvalidABAQSDocument(
        "Species \'" + id + "\' is missing the "
        "\'CELL_\' or \'AUTO_\' prefix."
      );
    }
  }

  void
  SpeciesList::storeCompilerSpecies(const std::string name,
    const CompilerSpecies::SpeciesType type)
  {
    CompilerSpecies sp(name);

    switch(type) {
      case CompilerSpecies::SpeciesType::cell:
        if(std::find(cells.begin(), cells.end(), sp) != cells.end()) {
          throw InvalidABAQSDocument(
            "Reuse of species \'CELL_" + name + "\'."
          );
        }
        cells.push_back(std::move(sp));
        break;
      case CompilerSpecies::SpeciesType::autoinducer:
        if(std::find(autoinducers.begin(), autoinducers.end(), sp) != autoinducers.end()) {
          throw InvalidABAQSDocument(
            "Redeclaration of species \'AUTO_" + name + "\'."
          );
        }
        autoinducers.push_back(std::move(sp));
        break;
    }
  }

  std::vector<CompilerSpecies>&
  SpeciesList::getCells()
  {
    return cells;
  }

  std::vector<CompilerSpecies>&
  SpeciesList::getAutoinducers()
  {
    return autoinducers;
  }
}
