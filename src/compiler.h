#ifndef COMPILER_INCLUDED
#define COMPILER_INCLUDED

#include "abaqs_arch.h"
#include "var_dep_graph.h"

#include <sbml/SBMLTypes.h>

#include <vector>
#include <stdexcept>

namespace abaqs {
  class Compiler {
    std::vector<abaqs::VarDepGraph> graphs;

    const libsbml::SBMLDocument * doc;
    const Architecture * arch;
  public:
    Compiler(const libsbml::SBMLDocument& doc,
      const abaqs::Architecture& arch);
    void run();
    void verify_valid_abaqs_doc(const libsbml::SBMLDocument& doc);
    void perform_var_dep_analysis(const libsbml::SBMLDocument& doc);
  };

  class InvalidABAQSDocument : public std::runtime_error
  {
  public:
    InvalidABAQSDocument(const char * string);
  };
}

#endif
