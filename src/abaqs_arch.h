#ifndef ABAQS_ARCH
#define ABAQS_ARCH

#include <stdexcept>

#ifdef __cplusplus
extern "C" {
#endif

  #include <libxml/tree.h>

#ifdef __cplusplus
}
#endif

namespace abaqs {
  class Architecture {
    void processXMLDoc(xmlDocPtr doc);
  public:
    Architecture(const char * filename);
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
