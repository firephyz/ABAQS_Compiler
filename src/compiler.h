#ifndef COMPILER_INCLUDED
#define COMPILER_INCLUDED

#include "abaqs_arch.h"
#include "var_dep_graph.h"
#include "rule_processor.h"
#include "verilog_writer.h"

#include <sbml/SBMLTypes.h>

#include <string>
#include <vector>
#include <stdexcept>

namespace abaqs {
  class Compiler {
    SpeciesRecord species;
    ParameterList parameters;
    RuleProcessor rproc;
    VerilogWriter output;

    const libsbml::Model * model;
    const Architecture * arch;
  public:
    Compiler(const libsbml::SBMLDocument& doc,
      const Architecture& arch);
    void run();
    void processSpecies();
    void processParameters();
    void processRules();


  };
}

#endif
