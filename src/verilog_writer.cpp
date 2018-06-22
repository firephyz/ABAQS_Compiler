#include "verilog_writer.h"
#include "compiler.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>

namespace abaqs {

  VerilogWriter::VerilogWriter(Compiler& comp)
    : compiler(comp)
  {}

  void
  VerilogWriter::run()
  {
    std::ifstream templ;
    templ.open("resources/rules_template.txt");

    /* 0 - Top generation comment
     * 1 - ASCS comment
     * 2 - Module and parameter declarations
     * 3 - IO declarations
     * 4 - Setup code
     * 6 - Generated code
     * 7 - end
     */
    std::vector<std::string> parts;
    while(!templ.eof()) {
      std::string result {gatherNextTemplateSection(templ)};
      parts.push_back(std::move(result));

      // Generated code needs to go in slot 6
      if(parts.size() == 5) {
        parts.push_back(generate());
      }
    }
    templ.close();

    if(parts.size() != 8) {
      throw CompilerRuntimeError(
        "Rules template has an invalid number of sections.");
    }

    std::ofstream out;
    out.open("out/cell_rules.v");    
    if(!out.good()) {
      throw CompilerRuntimeError(
        "Output directory \'out/\' does not exist. Create it.");
    }
    for(std::string& str : parts) {
      out << str << "\n";
    }
    out.close();

    //Finally done.
  }

  std::string
  VerilogWriter::gatherNextTemplateSection(std::ifstream& templ)
  {
    templ.seekg(0, templ.end);
    templ.get();
    return std::string("Testing\n");
  }

  std::string
  VerilogWriter::generate()
  {
    return std::string("");
  }

  CompilerRuntimeError::CompilerRuntimeError(std::string str)
  : std::runtime_error("Runtime error: " + str)
  {}
}
