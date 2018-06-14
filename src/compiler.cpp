#include "compiler.h"
#include "abaqs_arch.h"

#include <sbml/SBMLTypes.h>

#include <iostream>

namespace abaqs {

  Compiler::Compiler(const libsbml::SBMLDocument& doc,
    const abaqs::Architecture& arch)
  {

  }

  void
  Compiler::run()
  {
    std::cout << "Compiling..." << std::endl;
    return;
  }
}
