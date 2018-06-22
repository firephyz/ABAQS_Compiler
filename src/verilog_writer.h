#ifndef VERILOG_INCLUDED
#define VERILOG_INCLUDED

#include <string>
#include <stdexcept>

namespace abaqs {

  // Forward decl because compiler.h includes this
  // before the class declarations.
  class Compiler;

  class VerilogWriter {
    std::string gatherNextTemplateSection(std::ifstream& templ);
    std::string generate();

  public:
    Compiler& compiler;

    VerilogWriter(Compiler& comp);

    void run();
  };

  class CompilerRuntimeError : public std::runtime_error
  {
  public:
    CompilerRuntimeError(std::string str);
  };
}

#endif
