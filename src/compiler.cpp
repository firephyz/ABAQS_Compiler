#include "compiler.h"
#include "abaqs_arch.h"

#include <sbml/SBMLTypes.h>

#include <iostream>
#include <string>
#include <stdexcept>

namespace abaqs {

  Compiler::Compiler(const libsbml::SBMLDocument& doc0,
    const abaqs::Architecture& arch)
    : doc {&doc0}, arch {&arch}
  {
    // Don't proceed unless @doc contains correct members.
    Compiler::verify_valid_abaqs_doc(*doc);
  }

  void
  Compiler::run()
  {
    Compiler::perform_var_dep_analysis(*doc);
    return;
  }

  // Performs various surface level checks to ensure that the SBML Document
  // is valid for our architecture. Specifically, this only checks:
  // 1) We have one valid SIM_SPACE compartment
  // 2) We don't have any unitDefinitions, constraints, reactions or events.
  void
  Compiler::verify_valid_abaqs_doc(const libsbml::SBMLDocument& doc)
  {
    const libsbml::Model& model = *doc.getModel();

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

  void
  Compiler::perform_var_dep_analysis(const libsbml::SBMLDocument& doc)
  {

  }

  InvalidABAQSDocument::InvalidABAQSDocument(std::string str)
  : std::runtime_error("Document Error: " + str)
  {}
}
