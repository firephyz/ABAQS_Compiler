#include "abaqs_arch.h"

#ifdef __cplusplus
extern "C" {
#endif

  #include <libxml/parser.h>
  #include <libxml/tree.h>

#ifdef __cplusplus
}
#endif

#include <iostream>

namespace abaqs {

  void
  Architecture::printErrors()
  {
    std::cout << "Architecture errors..." << std::endl;
  }

  int
  Architecture::getNumErrors()
  {
    return 0;
  }

  ArchitectureReadFailure::ArchitectureReadFailure(const char * string)
  : std::runtime_error(string)
  {}

  Architecture
  readArchitectureFile(const char * file)
  {
    xmlParserCtxtPtr ctxt = NULL;
    xmlDocPtr doc = NULL;

    try {
      ctxt = xmlNewParserCtxt();
      if(ctxt == NULL) {
        throw ArchitectureReadFailure(
          "Failed to create new xml parser context.");
      }

      xmlDocPtr doc = xmlCtxtReadFile(ctxt, file, NULL, XML_PARSE_DTDVALID);

      if (doc == NULL) {
        throw ArchitectureReadFailure("Failed to parse architecture file.");
      }

      if (!ctxt->valid) {
        throw ArchitectureReadFailure("Invalid architecture file.");
      }

      return architectureFromXMLDoc(doc);

    } catch(ArchitectureReadFailure& failure) {
      if(ctxt != NULL) xmlFreeParserCtxt(ctxt);
      if(doc != NULL) xmlFreeDoc(doc);
      throw;
    }
  }

  Architecture
  architectureFromXMLDoc(xmlDocPtr doc)
  {
    return Architecture();
  }
}
