#include "verilog_writer.h"
#include "compiler.h"
#include "ast.h"

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
     * 5 - Generated code
     * 6 - end
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
      out << str;
    }
    out.close();

    //Finally done.
  }

  std::string
  VerilogWriter::gatherNextTemplateSection(
    std::ifstream& templ)
  {
    const std::string seperator {"<<<>>>\n\n"};

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
    std::string result {"/*\n * Generated code\n */\n\n"};

    //result += generateMoveDir();
    //result += generateMoveRq();
    // result += generateOutState();
    result += generateQuantity();

    return std::move(result);
  }

  std::string
  VerilogWriter::generateMoveDir()
  {
    std::string comment {"/* move_direction */\n"};

    const CompilerRule& rule = compiler.getRule("MOVE_DIR");
    std::string result = compiler.compileRule(rule);
    wrapInCombBlock(result);

    return std::move(comment + result);
  }

  std::string
  VerilogWriter::generateMoveRq()
  {
    std::string comment {"/* move_rq */\n"};

    const CompilerRule& rule = compiler.getRule("MOVE_RQ");
    std::string result = compiler.compileRule(rule);
    wrapInCombBlock(result);

    return std::move(comment + result);
  }

  // std::string
  // VerilogWriter::generateOutState()
  // {
  //   std::string comment {"/* cell_state_out */\n"};

  //   std::string result {"test\n"};
  //   wrapInCombBlock(result);

  //   return std::move(comment + result);
  // }

  std::string
  VerilogWriter::generateQuantity()
  {
    std::string comment {"/* quantity */\n"};

    const CompilerRule& rule = compiler.getRule("Q_OUT");
    std::string result = compiler.compileRule(rule);
    wrapInSeqBlock(result);

    return std::move(comment + result);
  }

  void
  wrapInCombBlock(std::string& result)
  {
    std::string prefix {"always@(*) begin\n"};
    std::string postfix {"end\n\n"};
    result.insert(0, prefix);
    result.insert(result.length(), postfix);
  }

  void
  wrapInSeqBlock(std::string& result)
  {
    std::string prefix {
      "always@(posedge clock or negedge "
      "reset_n) begin\n"};
    std::string postfix {"end\n\n"};
    result.insert(0, prefix);
    result.insert(result.length(), postfix);
  }
}
