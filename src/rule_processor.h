#ifndef RULE_PROCESSOR_INCLUDED
#define RULE_PROCESSOR_INCLUDED

#include "abaqs_types.h"

namespace abaqs {
  struct RuleProcessorData {
    SpeciesRecord * species;
    ParameterList * parameters;
  };

  class RuleProcessor {
  private:
    SpeciesRecord * species;
    ParameterList * parameters;

  public:
      void initMemberVariables(struct RuleProcessorData& data);
      void processRule(const libsbml::Rule& rule);
  };
}

#endif
