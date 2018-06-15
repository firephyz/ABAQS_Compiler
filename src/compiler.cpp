#include "compiler.h"
#include "abaqs_arch.h"
#include "doc_checks.h"

#include <sbml/SBMLTypes.h>

namespace abaqs {

  Compiler::Compiler(const libsbml::SBMLDocument& doc,
    const Architecture& arch)
    : arch {&arch}
  {
    model = doc.getModel();

    // libsbml seems to not detect some errors.
    // In the parameters list for example, writing
    // '<para' as a parameter is obviously invalid but doesn't
    // get caught during the error checks.
    if(model == nullptr) {
      throw InvalidABAQSDocument("Internal error. SBML model is null.");
    }
  }

  void
  Compiler::run()
  {
    // Don't proceed unless @doc contains correct members.
    verify_valid_abaqs_doc(*model);

    Compiler::processSpecies();
    Compiler::processParameters();
    Compiler::perform_var_dep_analysis();
    return;
  }

  void
  Compiler::perform_var_dep_analysis()
  {
    // TODO
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
}
