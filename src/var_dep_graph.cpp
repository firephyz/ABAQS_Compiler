#include "var_dep_graph.h"
#include "abaqs_types.h"

namespace abaqs {

  VarDepNode::VarDepNode(CompilerVar& var)
  : variable {var}
  {}

  void
  VarDepNode::add_var_dep(VarDepNode& node)
  {
    deps.push_back(&node);
  }
}
