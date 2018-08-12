#ifndef COMPILER_INCLUDED
#define COMPILER_INCLUDED

#include "abaqs_arch.h"
#include "rule_processor.h"
#include "verilog_writer.h"
#include "abaqs_types.h"
#include "rule.h"
#include "ast.h"
#include "inter_rep.h"

#include <sbml/SBMLTypes.h>

#include <string>
#include <vector>
#include <stdexcept>

namespace abaqs {
  class Compiler {
    SpeciesRecord species;
    std::vector<CompilerParameter> parameters;
    std::vector<CompilerFunction> functions;
    std::vector<CompilerInitAssignment> assigns;
    std::vector<CompilerRule> rules;

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
    void processInitAssignments();
    void processRules();
    void processFunctions();
    void record_parameter(const libsbml::Parameter& param);

    const CompilerRule& getRule(const std::string rule_name);
    std::string compileRule(const CompilerRule& rule);

    void linkComponentNames();
    void resolveFunctionNames(const std::string& name, ASTNode * node);
    const CompilerFunction * lookupCompilerFunction(
      const std::string& name);
    const CompilerFunction * lookupFunctionName(
      const std::string& name) const;
    double lookupParameterValue(const std::string& var) const;

    friend class RuleProcessor;
    friend class AST;
    friend class VerilogWriter;
  };

  class CompilerRuntimeError : public std::runtime_error
  {
  public:
    CompilerRuntimeError(std::string str);
  };
}

#endif
