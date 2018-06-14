#include "compiler.h"
#include "abaqs_arch.h"

#include <sbml/SBMLTypes.h>

#include <iostream>
#include <string>
#include <stdexcept>

namespace abaqs {

  Compiler::Compiler(const libsbml::SBMLDocument& doc,
    const abaqs::Architecture& arch)
    : model {doc.getModel()}, arch {&arch}
  {}

  void
  Compiler::run()
  {
    // Don't proceed unless @doc contains correct members.
    Compiler::verify_valid_abaqs_doc();
    Compiler::processSpecies();
    Compiler::perform_var_dep_analysis();
    return;
  }

  void
  Compiler::perform_var_dep_analysis()
  {
    // TODO
  }

  void
  Compiler::processSpecies()
  {
    const libsbml::ListOfSpecies * species = model->getListOfSpecies();
    for(uint i = 0; i < species->size(); ++i) {
      const libsbml::Species * sp = species->get(i);

      check_valid_species(*sp);

      Compiler::recordSpecies(*sp);
    }
  }

  void
  Compiler::recordSpecies(const libsbml::Species& sp)
  {
    // TODO
  }

  void
  check_valid_species(const libsbml::Species& sp) {
    // TODO Check and make sure these attributes below exist first.
    const std::string comp = sp.getCompartment();
    const bool hosu = sp.getHasOnlySubstanceUnits();
    const bool boundary = sp.getBoundaryCondition();
    const bool constant = sp.getConstant();

    const std::string name = sp.getId();
    if(comp.size() == 0) {
      throw InvalidABAQSDocument(
        "Species \'" + name + "\' must specify a compartment.");
    }
    if(comp != "SIM_SPACE") {
      throw InvalidABAQSDocument(
        "Species \'" + name + "\' must use compartment \'SIM_SPACE\'.");
    }
    if(hosu) {
      throw InvalidABAQSDocument(
        "Species \'" + name + "\' attribute \'hasOnlySubstanceUnits\'"
        " must be \'false\'.");
    }
    if(boundary) {
      throw InvalidABAQSDocument(
        "Species \'" + name + "\' boundary condition not currently supported.");
    }
    if(constant) {
      throw InvalidABAQSDocument(
        "Species \'" + name + "\' must not be constant.");
    }
  }

  // Performs various surface level checks to ensure that the SBML Document
  // is valid for our architecture. Specifically, this only checks:
  // 1) We have one valid SIM_SPACE compartment
  // 2) We don't have any unitDefinitions, constraints, reactions or events.
  void
  Compiler::verify_valid_abaqs_doc()
  {
    // Check the compartment
    check_valid_compartment(*model);

    if(model->getListOfUnitDefinitions()->size() != 0) {
      throw InvalidABAQSDocument("Unit definitions are not currently supported.");
    }
    if(model->getListOfConstraints()->size() != 0) {
      throw InvalidABAQSDocument("Constraints are not currently supported.");
    }
    if(model->getListOfReactions()->size() != 0) {
      throw InvalidABAQSDocument(
        "Reactions are not currently supported. Use rules instead.");
    }
    if(model->getListOfEvents()->size() != 0) {
      throw InvalidABAQSDocument("Events are not currently supported.");
    }

    return;
  }

  void check_valid_compartment(const libsbml::Model& model) {
    if(model.getCompartment(1) != nullptr) {
      throw InvalidABAQSDocument("Model must only contain the "
        "\'SIM_SPACE\' compartment.");
    }

    const libsbml::Compartment * comp = model.getCompartment("SIM_SPACE");
    if(comp == nullptr) {
      throw InvalidABAQSDocument("Model must contain the "
        "\'SIM_SPACE\' compartment.");
    }

    if(!comp->getConstant()) {
      throw InvalidABAQSDocument("\'SIM_SPACE\' compartment must be constant.");
    }
  }

  InvalidABAQSDocument::InvalidABAQSDocument(std::string str)
  : std::runtime_error("Document Error: " + str)
  {}
}
