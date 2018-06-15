#include "compiler.h"
#include "abaqs_arch.h"
#include "doc_checks.h"

#include <sbml/SBMLTypes.h>

namespace abaqs {

  Compiler::Compiler(const libsbml::SBMLDocument& doc,
    const abaqs::Architecture& arch)
    : model {doc.getModel()}, arch {&arch}
  {}

  void
  Compiler::run()
  {
    // Don't proceed unless @doc contains correct members.
    verify_valid_abaqs_doc(*model);
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
}
