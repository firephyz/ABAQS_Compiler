#include "compiler.h"
#include "abaqs_arch.h"
#include "doc_checks.h"
#include "rule_processor.h"
#include "verilog_writer.h"

#include <sbml/SBMLTypes.h>

namespace abaqs {

  Compiler::Compiler(const libsbml::SBMLDocument& doc,
    const Architecture& arch)
    : arch {&arch},
      rproc {*this}
  {
    model = doc.getModel();
    // libsbml seems to not detect some errors.
    // In the parameters list for example, writing
    // '<para' as a parameter is obviously invalid
    // for many reasons but doesn't get caught during
    // the error checks.
    // Also doesn't catch bad formatting in 'math' tags.
    if(model == nullptr) {
      throw InvalidABAQSDocument("Internal error. SBML model is null.");
    }
  }

  void
  Compiler::run()
  {
    // Don't proceed unless @doc contains correct members.
    verify_valid_abaqs_doc(*model);

    // Preprocess SBML file and get all the data
    // present inside the compiler.
    Compiler::processFunctions();
    Compiler::processSpecies();
    Compiler::processParameters();
    Compiler::processRules();

    output.generate();
  }

  void
  Compiler::processRules()
  {
    const libsbml::ListOfRules * rules = model->getListOfRules();

    for(uint i = 0; i < rules->size(); ++i) {
      const libsbml::Rule * rule = rules->get(i);

      // Do requisite rule processing.
      // Generate dependency graphs, etc.
      rproc.processRule(*rule);
    }
  }

  void
  Compiler::processParameters()
  {
    const libsbml::ListOfParameters * params = model->getListOfParameters();
    for(uint i = 0; i < params->size(); ++i) {
      const libsbml::Parameter * p = params->get(i);

      check_valid_parameter(*p);

      parameters.record(*p);
    }

    //TODO
    // We need to ensure parameters required by ABAQS are present.
  }

  void
  Compiler::processSpecies()
  {
    const libsbml::ListOfSpecies * sps = model->getListOfSpecies();
    for(uint i = 0; i < sps->size(); ++i) {
      const libsbml::Species * sp = sps->get(i);

      check_valid_species(*sp);

      species.record(*sp);
    }
  }

  void
  Compiler::processFunctions()
  {
    const libsbml::ListOfFunctionDefinitions * funcs =
      model->getListOfFunctionDefinitions();
    for(uint i = 0; i < funcs->size(); ++i) {
      const libsbml::FunctionDefinition * func = funcs->get(i);

      check_valid_function(*func);

      functions.record(*func);
    }
  }
}
