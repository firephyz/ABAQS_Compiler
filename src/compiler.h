#ifndef COMPILER_INCLUDED
#define COMPILER_INCLUDED

#include "abaqs_arch.h"

#include <sbml/SBMLTypes.h>

namespace abaqs {
  class Compiler {
  public:
    Compiler(const libsbml::SBMLDocument& doc,
      const abaqs::Architecture& arch);
    void run();
  };
}

#endif
