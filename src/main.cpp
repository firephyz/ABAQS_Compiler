#include <iostream>
#include <sbml/SBMLTypes.h>

int main(int argc, char * argv[]) {

  if(argc != 2) {
    std::cerr << "Usage: abaqs <source file>" << std::endl;
    return 1;
  }

  try {
    libsbml::SBMLDocument& doc = *libsbml::readSBMLFromFile(argv[1]);
    // If we encounter errors, we can't move on. Report to user.
    if(doc.getNumErrors() > 0) {
      doc.printErrors();
      return 1;
    }
    else {
      std::cout << "Everything is good." << std::endl;
    }
  } catch (...) {
    std::cerr << "Unknown exception.";
    return 1;
  }

  return 0;
}
