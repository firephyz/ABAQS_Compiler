#ifndef ABAQS_ARCH
#define ABAQS_ARCH

#include <stdexcept>

#include <libxml/tree.h>

namespace abaqs {
  class Architecture {
  public:
    int getNumErrors();
    void printErrors();
  };

  class ArchitectureReadFailure : public std::runtime_error
  {
  public:
    ArchitectureReadFailure(const char * string);
  };

  Architecture readArchitectureFile(const char * file);
  Architecture architectureFromXMLDoc(xmlDocPtr doc);
}

#endif
