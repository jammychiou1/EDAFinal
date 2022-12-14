#ifndef __CIRCUIT_H__
#define __CIRCUIT_H__

#include <map>
#include <set>
#include <string>
#include <vector>

#include "dag.h"
#include "math_util.h"

typedef std::vector<bool> BitVec;

enum Type { AND, OR, NAND, NOR, NOT, BUF, XOR, XNOR, NET, INPUT, OUTPUT };

class Circuit {
 private:
  struct Component {
    std::string name;
    Type type;
    int node_idx;
  };

  std::map<std::string, Component> m_components;
  DAG<BitVec> m_graph;

  std::map<std::string, int> m_inputs;
  std::map<std::string, int> m_outputs;
  std::map<std::string, int> m_widths;

  void m_add_wire(std::string wire_name);

 public:
  void add_word(std::string word_name, int width);
  void add_gate(std::string gate_name, Type type, std::string input_name,
                std::vector<std::string> output_names);
  void def_input(std::string input_name, int width);
  void def_output(std::string output_name, int width);
  void commit();

  const std::map<std::string, int> &get_input_widths() { return m_inputs; }
  const std::map<std::string, int> &get_output_widths() { return m_outputs; }

  std::pair<std::map<std::string, std::vector<BigInt>>,
            std::map<std::string, std::vector<BigInt>>>
  generate_simulation(std::string control_input = "", long long val = -1);
};

#endif  // __CIRCUIT_H__
