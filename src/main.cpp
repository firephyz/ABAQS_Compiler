#if __cplusplus < 201703L
#error "We require a C++17 compiler."
#endif

#include <iostream>

#include <sbml/SBMLTypes.h>

#include "abaqs_arch.h"
#include "compiler.h"
#include "doc_checks.h"

#include <string>

int main(int argc, char * argv[]) {

  if(argc != 3) {
    std::cerr << "Usage: abaqs <arch file> <source file>" << std::endl;
    return 1;
  }

  try {
    abaqs::Architecture arch(argv[1]);
    libsbml::SBMLDocument doc = *libsbml::readSBMLFromFile(argv[2]);

    // If we encounter errors, we can't move on. Report to user.
    if(arch.getNumErrors() > 0 || doc.getNumErrors() > 0) {
      if(arch.getNumErrors() > 0) arch.printErrors();
      if(doc.getNumErrors() > 0) doc.printErrors();
      return 1;
    }

    // Don't let documents without SBML version 3.2 support compile.
    if(doc.checkL3v2Compatibility()) {
      doc.printErrors();
      return 1;
    }

    abaqs::Compiler compiler(doc, arch);
    compiler.run();
  }
  catch (abaqs::InvalidABAQSDocument& error) {
    std::cerr << error.what() << std::endl;
    return 1;
  }
  catch (abaqs::ArchitectureReadFailure& failure) {
    std::cerr << failure.what() << std::endl;
    return 1;
  }
  catch (abaqs::CompilerRuntimeError& error) {
    std::cerr << error.what() << std::endl;
    return 1;
  }
  catch(...) {
    std::cerr << "Unknown exception.\n";
    return 1;
  }

  return 0;
}
