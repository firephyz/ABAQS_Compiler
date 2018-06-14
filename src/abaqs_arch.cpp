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

  Architecture::Architecture(const char * filename) {
    xmlParserCtxtPtr ctxt = NULL;
    xmlDocPtr doc = NULL;

    try {
      ctxt = xmlNewParserCtxt();
      if(ctxt == NULL) {
        throw ArchitectureReadFailure(
          "Failed to create new xml parser context.");
      }

      xmlDocPtr doc = xmlCtxtReadFile(ctxt, filename, NULL, XML_PARSE_DTDATTR);

      if (doc == NULL) {
        throw ArchitectureReadFailure("Failed to parse architecture file.");
      }

      if (!ctxt->valid) {
        throw ArchitectureReadFailure("Invalid architecture file.");
      }

      Architecture::processXMLDoc(doc);

    } catch(ArchitectureReadFailure& failure) {
      if(ctxt != NULL) xmlFreeParserCtxt(ctxt);
      if(doc != NULL) xmlFreeDoc(doc);
      throw;
    }
  }

  void Architecture::processXMLDoc(xmlDocPtr doc)
  {
    std::cout << "Processing architecture XML." << std::endl;
    return;
  }

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
}
