#include "var_dep_graph.h"

namespace abaqs {

  int CompilerVar::id_counter = 0;

  CompilerVar::CompilerVar(const char * var_name)
  : name {var_name}, is_constant {false}, id {id_counter}
  {
    ++id_counter;
  }

  CompilerVar::CompilerVar(const char * var_name, double init_value)
  : CompilerVar(var_name)
  {
    is_constant = true;
  }

  VarDepNode::VarDepNode(CompilerVar& var)
  : variable {var}
  {}

  void
  VarDepNode::add_var_dep(VarDepNode& node)
  {
    deps.push_back(&node);
  }
}
