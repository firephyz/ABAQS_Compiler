#include "doc_checks.h"

#include <sbml/SBMLTypes.h>

#include <iostream>
#include <stdexcept>
#include <string>

namespace abaqs {
  InvalidABAQSDocument::InvalidABAQSDocument(std::string str)
  : std::runtime_error("Document Error: " + str)
  {}

  // Performs various surface level checks to ensure that the SBML Document
  // is valid for our architecture. Specifically, this only checks:
  // 1) We have one valid SIM_SPACE compartment
  // 2) We don't have any unitDefinitions, constraints, reactions or events.
  void
  verify_valid_abaqs_doc(const libsbml::Model& model)
  {
    // Check the compartment
    check_valid_compartment(model);

    if(model.getListOfUnitDefinitions()->size() != 0) {
      throw InvalidABAQSDocument("Unit definitions are not currently supported.");
    }
    if(model.getListOfConstraints()->size() != 0) {
      throw InvalidABAQSDocument("Constraints are not currently supported.");
    }
    if(model.getListOfReactions()->size() != 0) {
      throw InvalidABAQSDocument(
        "Reactions are not currently supported. Use rules instead.");
    }
    if(model.getListOfEvents()->size() != 0) {
      throw InvalidABAQSDocument("Events are not currently supported.");
    }

    return;
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

  void
  check_valid_compartment(const libsbml::Model& model)
  {
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
}
