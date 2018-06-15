#include "abaqs_types.h"
#include "doc_checks.h"

#include <sbml/SBMLTypes.h>

#include <vector>
#include <string>
#include <utility>

namespace abaqs {

  CompilerSpecies::CompilerSpecies(const std::string name)
  : name {std::move(name)}
  {}

  bool
  CompilerSpecies::operator==(const CompilerSpecies& sp)
  {
    return name == sp.name;
  }

  void
  SpeciesRecord::record(const libsbml::Species& sp)
  {
    const std::string cell_prefix {"CELL_"};
    const std::string auto_prefix {"AUTO_"};
    std::string id = sp.getId();

    if(id.find(cell_prefix) != std::string::npos) {
      SpeciesRecord::storeCompilerSpecies(id,
        CompilerSpecies::SpeciesType::cell);
    }
    else if(id.find(auto_prefix) != std::string::npos) {
      SpeciesRecord::storeCompilerSpecies(id,
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

  CompilerParameter::CompilerParameter(const std::string name,
                                       double value,
                                       const bool is_constant)
  : name {std::move(name)},
    value {value},
    is_constant {is_constant},
    value_is_valid {true}
  {}

  CompilerParameter::CompilerParameter(const std::string name,
                                       const bool is_constant)
  : name {std::move(name)},
  is_constant {is_constant},
  value_is_valid {false}
  {}

  bool
  CompilerParameter::operator==(const CompilerParameter& p)
  {
    return name == p.name;
  }

  void
  ParameterList::record(const libsbml::Parameter& p)
  {
    CompilerParameter * par = nullptr;
    const std::string name = p.getId();
    const bool constant = p.getConstant();

    if(!p.isSetValue()) {
      par = new CompilerParameter(name, constant);
    }
    else {
      const double value = p.getValue();
      par = new CompilerParameter(name, value, constant);
    }

    if(std::find(begin(), end(), *par) != end()) {
      throw InvalidABAQSDocument(
        "Redeclaration of parameter \'" + name + "\'.");
    }

    push_back(std::move(*par));
  }
}
