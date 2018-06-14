#ifndef VAR_DEP_GRAPH_INCLUDED
#define VAR_DEP_GRAPH_INCLUDED

#include <string>
#include <vector>

namespace abaqs {

  class CompilerVar {
    static int id_counter;
    bool is_constant;
    // Value only valid if is_constant=true
    double value;

  public:
    CompilerVar(const char * var_name, double value); // Assume variable is constant
    CompilerVar(const char * var_name); // Variable is not constant

    const int id;
    const std::string name;
  };

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
