#include <iostream>

#include <sbml/SBMLTypes.h>

#include "abaqs_arch.h"

int main(int argc, char * argv[]) {

  if(argc != 3) {
    std::cerr << "Usage: abaqs <arch file> <source file>" << std::endl;
    return 1;
  }

  try {
    abaqs::Architecture arch = abaqs::readArchitectureFile(argv[1]);
    libsbml::SBMLDocument doc = *libsbml::readSBMLFromFile(argv[2]);

    // If we encounter errors, we can't move on. Report to user.
    if(arch.getNumErrors() > 0 || doc.getNumErrors() > 0) {
      if(arch.getNumErrors() > 0) arch.printErrors();
      if(doc.getNumErrors() > 0) doc.printErrors();
      return 1;
    }

    std::cout << "Everything is good." << std::endl;
  }
  catch(abaqs::ArchitectureReadFailure& failure) {
    std::cerr << failure.what() << std::endl;
    return 1;
  }
  catch(...) {
    std::cerr << "Unknown exception.";
    return 1;
  }

  return 0;
}
