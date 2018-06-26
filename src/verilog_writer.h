#ifndef VERILOG_INCLUDED
#define VERILOG_INCLUDED

#include "ast.h"

#include <string>
#include <stdexcept>

namespace abaqs {

  // Forward decl because compiler.h includes this
  // before the class declarations.
  class Compiler;

  class VerilogWriter {
    std::string gatherNextTemplateSection(std::ifstream& templ);
    std::string generate();
    std::string generateMoveDir();
    std::string generateMoveRq();
    std::string generateQuantity();
    // std::string generateOutState();

  public:
    Compiler& compiler;

    VerilogWriter(Compiler& comp);

    void run();
  };

  void wrapInCombBlock(std::string& result);
  void wrapInSeqBlock(std::string& result);
}

#endif
