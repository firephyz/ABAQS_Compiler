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

    if(parts.size() != 7) {
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
  VerilogWriter::gatherNextTemplateSection(
    std::ifstream& templ)
  {
    const std::string seperator {"\n<<<>>>\n\n"};

    std::string result;

    while(!templ.eof()) {
      result.push_back(templ.get());
      int index = result.length() - seperator.length();
      if(index < 0) continue;
      std::string end = result.substr(index);
      if(seperator == end) {
        return result.substr(0, index);
      }
    }

    // Last character will be eof. We don't need that.
    result.erase(result.length() - 1, result.npos);
    return std::move(result);
  }

  std::string
  VerilogWriter::generate()
  {
    return std::string("Hello world.");
  }

  CompilerRuntimeError::CompilerRuntimeError(std::string str)
  : std::runtime_error("Runtime error: " + str)
  {}
}
