#include "circuit.h"
using namespace std;

independent_bits_engine<mt19937, 1, mt19937::result_type> bit_gen;

string gen_wire_name(string word_name, int width, int idx) {
  if (width == -1) {
    assert(idx == 0);
    return word_name;
  }
  return word_name + '[' + to_string(idx) + ']';
}

vector<string> gen_wire_names(string word_name, int width) {
  if (width == -1) {
    return {gen_wire_name(word_name, width, 0)};
  }
  vector<string> wire_names;
  for (int i = 0; i < width; i++) {
    wire_names.push_back(gen_wire_name(word_name, width, i));
  }
  return wire_names;
}

void Circuit::m_add_wire(string wire_name) {
  assert(m_node_idxs.count(wire_name) == 0);

  int idx = m_graph.add_node(Component{wire_name, NET, {}});
  m_node_idxs[wire_name] = idx;
}

void Circuit::add_word(string word_name, int width) {
  assert(m_widths.count(word_name) == 0);
  // cerr << word_name << ' ' << width << '\n';
  for (string wire_name : gen_wire_names(word_name, width)) {
    m_add_wire(wire_name);
  }
  m_widths[word_name] = width;
}

void Circuit::add_gate(string gate_name, Type type, string output_name,
                       vector<string> input_names) {
  // cerr << gate_name << '\n';
  // cerr << output_name << '\n';
  // for (string input_name : input_names) {
  //   cerr << input_name << ' ';
  // }
  // cerr << '\n';

  assert(m_node_idxs.count(gate_name) == 0);

  int idx_u = m_graph.add_node(Component{gate_name, type, {}});

  assert(m_node_idxs.count(output_name));
  int idx_v_o = m_node_idxs[output_name];
  m_graph.add_edge(idx_u, idx_v_o);

  for (string input_name : input_names) {
    assert(m_node_idxs.count(input_name));
    int idx_v_i = m_node_idxs[input_name];
    m_graph.add_edge(idx_v_i, idx_u);
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

void Circuit::def_const(std::string const_name, int width, long long val) {
  m_consts[const_name] = {width, val};
  vector<string> wire_names = gen_wire_names(const_name, width);
  for (string wire_name : wire_names) {
    int idx = m_graph.add_node(Component{wire_name, CONST, {}});
    m_node_idxs[wire_name] = idx;
  }
}

void Circuit::commit() {
  for (auto [input_name, input_width] : m_inputs) {
    assert(m_widths[input_name] == input_width);
    vector<string> wire_names = gen_wire_names(input_name, input_width);
    for (string wire_name : wire_names) {
      assert(m_node_idxs.count(wire_name));
      m_graph.get_node(m_node_idxs[wire_name]).type = INPUT;
    }
  }
  for (auto [output_name, output_width] : m_outputs) {
    assert(m_widths[output_name] == output_width);
    vector<string> wire_names = gen_wire_names(output_name, output_width);
    for (string wire_name : wire_names) {
      assert(m_node_idxs.count(wire_name));
      m_graph.get_node(m_node_idxs[wire_name]).type = OUTPUT;
    }
  }
  m_graph.commit();
}

BitVec vec_buf(const BitVec &v) {
  return v;
}

BitVec vec_not(const BitVec &v) {
  BitVec out(v.size());
  for (int i = 0; i < v.size(); i++) {
    out[i] = !v[i];
  }
  return out;
}

BitVec vec_and(const BitVec &v1, const BitVec &v2) {
  assert(v1.size() == v2.size());
  BitVec out(v1.size());
  for (int i = 0; i < v1.size(); i++) {
    out[i] = v1[i] && v2[i];
  }
  return out;
}

BitVec vec_or(const BitVec &v1, const BitVec &v2) {
  assert(v1.size() == v2.size());
  BitVec out(v1.size());
  for (int i = 0; i < v1.size(); i++) {
    out[i] = v1[i] || v2[i];
  }
  return out;
}

BitVec vec_xor(const BitVec &v1, const BitVec &v2) {
  assert(v1.size() == v2.size());
  BitVec out(v1.size());
  for (int i = 0; i < v1.size(); i++) {
    out[i] = v1[i] ^ v2[i];
  }
  return out;
}

BitVec vec_nand(const BitVec &v1, const BitVec &v2) {
  return vec_not(vec_and(v1, v2));
}

BitVec vec_nor(const BitVec &v1, const BitVec &v2) {
  return vec_not(vec_or(v1, v2));
}

BitVec vec_xnor(const BitVec &v1, const BitVec &v2) {
  return vec_not(vec_xor(v1, v2));
}

pair<map<string, vector<BigInt>>, map<string, vector<BigInt>>>
Circuit::generate_simulation(int num_samples, map<string, long long> controls) {

  for (int idx : m_graph.topo_ord_to_idx()) {
    assert(m_graph.get_node(idx).data.empty());
  }

  for (auto [const_name, p] : m_consts) {
    auto [width, val] = p;
    vector<string> wire_names = gen_wire_names(const_name, width);
    for (int i = 0; i < wire_names.size(); i++) {
      auto wire_hdl = m_node_idxs[wire_names[i]];
      m_graph.get_node(wire_hdl).data = BitVec(num_samples, val & (1ll << i));
    }
  }

  for (auto [input_name, input_width] : m_inputs) {
    vector<string> wire_names = gen_wire_names(input_name, input_width);
    for (int i = 0; i < wire_names.size(); i++) {
      // cerr << wire_names[i] << '\n';
      assert(m_node_idxs.count(wire_names[i]));
      int wire_idx = m_node_idxs[wire_names[i]];
      if (controls.count(input_name)) {
        long long val = controls[input_name];
        m_graph.get_node(wire_idx).data = BitVec(num_samples, val & (1ll << i));
      }
      else {
        for (int t = 0; t < num_samples; t++) {
          m_graph.get_node(wire_idx).data.push_back(bit_gen());
        }
      }
    }
  }

  auto node = [&](int idx) -> Component& { return m_graph.get_node(idx); };

  for (int idx : m_graph.topo_ord_to_idx()) {
    // assert(hdl->data.empty());
    vector<int> ins = m_graph.in_neighbours(idx);
    if (node(idx).type == INPUT || node(idx).type == CONST) {
      continue;
    }
    if (node(idx).type == NET || node(idx).type == BUF || node(idx).type == OUTPUT) {
      assert(ins.size() == 1);
      node(idx).data = node(ins[0]).data;
      continue;
    }
    if (node(idx).type == NOT) {
      assert(ins.size() == 1);
      node(idx).data = vec_not(node(ins[0]).data);
      continue;
    }
    if (node(idx).type == AND) {
      assert(ins.size() == 2);
      node(idx).data = vec_and(node(ins[0]).data, node(ins[1]).data);
      continue;
    }
    if (node(idx).type == OR) {
      assert(ins.size() == 2);
      node(idx).data = vec_or(node(ins[0]).data, node(ins[1]).data);
      continue;
    }
    if (node(idx).type == NAND) {
      assert(ins.size() == 2);
      node(idx).data = vec_nand(node(ins[0]).data, node(ins[1]).data);
      continue;
    }
    if (node(idx).type == NOR) {
      assert(ins.size() == 2);
      node(idx).data = vec_nor(node(ins[0]).data, node(ins[1]).data);
      continue;
    }
    if (node(idx).type == XOR) {
      assert(ins.size() == 2);
      node(idx).data = vec_xor(node(ins[0]).data, node(ins[1]).data);
      continue;
    }
    if (node(idx).type == XNOR) {
      assert(ins.size() == 2);
      node(idx).data = vec_xnor(node(ins[0]).data, node(ins[1]).data);
      continue;
    }
    assert(false);
  }

  map<string, vector<BigInt>> input_data, output_data;
  for (auto [input_name, input_width] : m_inputs) {
    vector<string> wire_names = gen_wire_names(input_name, input_width);
    input_data[input_name].resize(num_samples);
    for (int i = 0; i < wire_names.size(); i++) {
      int wire_idx = m_node_idxs[wire_names[i]];
      for (int t = 0; t < num_samples; t++) {
        if (m_graph.get_node(wire_idx).data[t]) {
          input_data[input_name][t].setbit(i);
        }
      }
    }
  }
  for (auto [output_name, output_width] : m_outputs) {
    vector<string> wire_names = gen_wire_names(output_name, output_width);
    output_data[output_name].resize(num_samples);
    for (int i = 0; i < wire_names.size(); i++) {
      int wire_idx = m_node_idxs[wire_names[i]];
      for (int t = 0; t < num_samples; t++) {
        if (m_graph.get_node(wire_idx).data[t]) {
          output_data[output_name][t].setbit(i);
        }
      }
    }
  }

  for (int idx : m_graph.topo_ord_to_idx()) {
    m_graph.get_node(idx).data.clear();
  }

  return {input_data, output_data};
}
