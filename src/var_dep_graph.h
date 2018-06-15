#ifndef VAR_DEP_GRAPH_INCLUDED
#define VAR_DEP_GRAPH_INCLUDED

#include <string>
#include <vector>

#include "abaqs_types.h"

namespace abaqs {

  class VarDepNode {
    std::vector<VarDepNode *> deps;
  public:
    VarDepNode(CompilerVar& var);
    void add_var_dep(VarDepNode& node);

    const CompilerVar& variable;
  };

  class VarDepGraph {
  public:

  };
}

#endif
