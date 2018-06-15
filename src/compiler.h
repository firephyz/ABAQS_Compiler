#ifndef COMPILER_INCLUDED
#define COMPILER_INCLUDED

#include "abaqs_arch.h"
#include "var_dep_graph.h"

#include <sbml/SBMLTypes.h>

#include <string>
#include <vector>
#include <stdexcept>

namespace abaqs {
  class Compiler {
    std::vector<abaqs::VarDepGraph> graphs;
    abaqs::SpeciesList species;

    const libsbml::Model * model;
    const Architecture * arch;
  public:
    Compiler(const libsbml::SBMLDocument& doc,
      const abaqs::Architecture& arch);
    void run();
    void processSpecies();
    void perform_var_dep_analysis();


  };
}

#endif
