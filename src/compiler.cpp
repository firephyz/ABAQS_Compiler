#include "compiler.h"
#include "abaqs_arch.h"

#include <sbml/SBMLTypes.h>

#include <iostream>
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

  void
  Compiler::verify_valid_abaqs_doc(const libsbml::SBMLDocument& doc)
  {
    const libsbml::Model * model = doc.getModel();

    return;
  }

  void
  Compiler::perform_var_dep_analysis(const libsbml::SBMLDocument& doc)
  {

  }

  InvalidABAQSDocument::InvalidABAQSDocument(const char * string)
  : std::runtime_error(string)
  {}
}
