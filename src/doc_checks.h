#ifndef DOC_CHECKS_INCLUDED
#define DOC_CHECKS_INCLUDED

#include <sbml/SBMLTypes.h>

namespace abaqs {

  class InvalidABAQSDocument : public std::runtime_error
  {
  public:
    InvalidABAQSDocument(std::string str);
  };

  void verify_valid_abaqs_doc(const libsbml::Model& model);
  void check_valid_compartment(const libsbml::Model& model);
  void check_valid_species(const libsbml::Species& sp);
  void check_valid_parameter(const libsbml::Parameter& p);
  void check_valid_function(const libsbml::FunctionDefinition& func);
  void check_valid_rule(const libsbml::Rule& rule);
}

#endif
