#include "compiler.h"
#include "abaqs_arch.h"
#include "doc_checks.h"
#include "rule_processor.h"
#include "verilog_writer.h"

#include <sbml/SBMLTypes.h>

namespace abaqs {

  Compiler::Compiler(const libsbml::SBMLDocument& doc,
    const Architecture& arch)
    : rproc {*this},
      arch {&arch}
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
    processFunctions();
    processSpecies();
    processParameters();
    processInitAssignments();
    processRules();

    for(auto& func : functions) {
      std::cout << func << std::endl;
    }

    for(auto& rule : rules) {
      std::cout << rule << std::endl; 
    }

    output.generate();
  }

  void
  Compiler::processInitAssignments()
  {
    const libsbml::ListOfInitialAssignments * assigns =
      model->getListOfInitialAssignments();
    
    for(uint i = 0; i < assigns->size(); ++i) {
      const libsbml::InitialAssignment * init = assigns->get(i);

      Compiler::assigns.push_back(CompilerInitAssignment(*init));
    }
  }

  void
  Compiler::processRules()
  {
    const libsbml::ListOfRules * rules = model->getListOfRules();

    for(uint i = 0; i < rules->size(); ++i) {
      const libsbml::Rule * rule = rules->get(i);

      check_valid_rule(*rule);

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

      record_parameter(*p);
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

      CompilerFunction abaqs_func(*func);

      if(std::find(functions.begin(),
                   functions.end(),
                   abaqs_func) != functions.end()) {
        throw InvalidABAQSDocument(
          "Redeclaration of function \'" + abaqs_func.var_name + "\'.");
      }

      functions.push_back(std::move(abaqs_func));
    }
  }

  void
  Compiler::record_parameter(const libsbml::Parameter& param)
  {
    CompilerParameter par;
    const std::string name = param.getId();
    const bool constant = param.getConstant();

    if(!param.isSetValue()) {
      par = CompilerParameter(name, constant);
    }
    else {
      const double value = param.getValue();
      par = CompilerParameter(name, value, constant);
    }

    if(std::find(parameters.begin(),
                 parameters.end(),
                 par) != parameters.end()) {
      throw InvalidABAQSDocument(
        "Redeclaration of parameter \'" + name + "\'.");
    }

    parameters.push_back(std::move(par));
  }
}
