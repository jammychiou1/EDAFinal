#ifndef __CIRCUIT_H__
#define __CIRCUIT_H__

#include <map>
#include <random>
#include <set>
#include <string>
#include <vector>

#include "dag.h"
#include "math_util.h"

typedef std::vector<bool> BitVec;

enum Type { AND, OR, NAND, NOR, NOT, BUF, XOR, XNOR, NET, INPUT, OUTPUT, CONST };

extern std::independent_bits_engine<std::mt19937, 1, std::mt19937::result_type> bit_gen;

class Circuit {
 private:

  // TODO: change data to use BigInt
  struct Component {
    std::string name;
    Type type;
    BitVec data;
  };

  std::map<std::string, int> m_node_idxs;
  DAG<Component> m_graph;

  std::map<std::string, int> m_inputs;
  std::map<std::string, int> m_outputs;
  std::map<std::string, std::pair<int, long long>> m_consts;
  std::map<std::string, int> m_widths;

  void m_add_wire(std::string wire_name);

 public:
  void add_word(std::string word_name, int width);
  void add_gate(std::string gate_name, Type type, std::string input_name,
                std::vector<std::string> output_names);
  void def_input(std::string input_name, int width);
  void def_output(std::string output_name, int width);
  void def_const(std::string const_name, int width, long long val);
  void commit();

  const std::map<std::string, int> &get_input_widths() { return m_inputs; }
  const std::map<std::string, int> &get_output_widths() { return m_outputs; }

  std::pair<std::map<std::string, std::vector<BigInt>>,
            std::map<std::string, std::vector<BigInt>>>
  generate_simulation(int num_samples, std::map<std::string, long long> controls = {});
};

#endif  // __CIRCUIT_H__
