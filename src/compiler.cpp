#include "compiler.h"
#include "abaqs_arch.h"
#include "doc_checks.h"
#include "rule_processor.h"
#include "verilog_writer.h"
#include "rule.h"
#include "inter_rep.h"

#include <sbml/SBMLTypes.h>

namespace abaqs {

  Compiler::Compiler(const libsbml::SBMLDocument& doc,
    const Architecture& arch)
    : rproc {*this}
    , output {*this}
    ,  arch {&arch}
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

    output.run();
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

    // See ABAQS_Source/src/cell_rules.v to get more understanding
    // for the parameters.
    std::vector<std::string> req_params = {
      "Q_OUT",
      "MOVE_DIR",
      "MOVE_RQ"
    };
    for(auto& str : req_params) {
      const auto& element = std::find_if(
        parameters.begin(),
        parameters.end(),
        [str](CompilerParameter& param) {
          return param.name == str;
      });

      if(element == parameters.end()) {
        throw InvalidABAQSDocument(
          "Missing ABAQS required parameter: " + str); 
      }
    }
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

  const CompilerRule&
  Compiler::getRule(const std::string rule_name)
  {
    for(const CompilerRule& rule : rules) {
      if(rule.var_name == rule_name) {
        return rule;
      }
    }

    throw CompilerRuntimeError(
      "Cannot find definition of rule \'" +
      rule_name + "\'.");
  }

  std::string
  Compiler::compileRule(const CompilerRule& rule)
  {
    std::string result;

    const InterRep ir(*this, rule.math);
    for(auto& statement : ir.statements) {
      result += statement.toString() + "\n";
    }

    // Final variable assignment isn't included in the rule's ast
    const IRStatement& last = 
      ir.statements[ir.statements.size() - 1];
    result += rule.var_name +
              " = " +
              last.target +
              ";\n";
    
    return std::move(result);
  }

  CompilerRuntimeError::CompilerRuntimeError(std::string str)
  : std::runtime_error("Runtime error: " + str)
  {}
}
