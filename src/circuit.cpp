#include "circuit.h"
using namespace std;

void Circuit::m_add_wire(string wire_name) {
  assert(m_components.count(wire_name) == 0);

  int u = m_graph.add_node();
  m_components[wire_name].name = wire_name;
  m_components[wire_name].type = NET;
  m_components[wire_name].node_idx = u;
}

void Circuit::add_word(string word_name, int width) {
  assert(m_widths.count(word_name) == 0);

  vector<string> wire_names;
  if (width == -1) {
    string wire_name = word_name;
    m_add_wire(wire_name);
    wire_names.push_back(wire_name);
  } else {
    for (int i = 0; i < width; i++) {
      string wire_name = word_name + '[' + to_string(i) + ']';
      m_add_wire(wire_name);
      wire_names.push_back(wire_name);
    }
  }

  m_widths[word_name] = width;
}

void Circuit::add_gate(string gate_name, Type type, string input_name,
                       vector<string> output_names) {
  assert(m_components.count(gate_name) == 0);

  int u = m_graph.add_node();
  m_components[gate_name].name = gate_name;
  m_components[gate_name].type = type;
  m_components[gate_name].node_idx = u;

  int v_i = m_components[input_name].node_idx;
  m_graph.add_edge(v_i, u);

  for (string output_name : output_names) {
    int v_o = m_components[output_name].node_idx;
    m_graph.add_edge(u, v_o);
  }
}

void Circuit::def_input(string input_name, int width) {
  assert(m_inputs.count(input_name) == 0);
  m_inputs[input_name] = width;
}

void Circuit::def_output(std::string output_name, int width) {
  assert(m_outputs.count(output_name) == 0);
  m_outputs[output_name] = width;
}

void Circuit::commit() {
  m_graph.commit();
  // TODO: check input output word consistency
}

pair<map<string, vector<BigInt>>, map<string, vector<BigInt>>>
Circuit::generate_simulation(string control_input, long long val) {
  return {};
}
