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

    const libsbml::Model * model;
    const Architecture * arch;
  public:
    Compiler(const libsbml::SBMLDocument& doc,
      const abaqs::Architecture& arch);
    void run();
    void verify_valid_abaqs_doc();
    void processSpecies();
    void recordSpecies(const libsbml::Species& sp);
    void perform_var_dep_analysis();
  };

  class InvalidABAQSDocument : public std::runtime_error
  {
  public:
    InvalidABAQSDocument(std::string str);
  };

  void check_valid_compartment(const libsbml::Model& model);
  void check_valid_species(const libsbml::Species& sp);
}

#endif
