#include "simulator.h"

#include <cassert>
#include <queue>

using namespace std;

const bool DEBUG_MSG = false;

void Simulator::generate_output() {
  if (DEBUG_MSG) {
    cout << "start generate output..." << endl;
  }
  output_testcase.clear();
  for (auto [name, width] : input_infos) {
    if (width == -1) {
      // from [[0], [1], [1]] to [0, 1, 1]
      BitVec tmp;
      for (int i = 0; i < it->second.size(); i++)
        tmp.push_back(it->second[i][0]);
      in->ins[0]->value = tmp;
      process.push(in->ins[0]);
    } else {
      // from [[0, 1, 1], [1, 0, 1], [1, 0, 0]]
      // to [0, 1, 1], [1, 0, 0], [1, 1, 0]
      for (int i = 0; i < in->width; i++) {
        BitVec tmp;
        for (int j = 0; j < it->second.size(); j++) {
          tmp.push_back(it->second[j][in->width - i - 1]);
        }
        in->ins[i]->value = tmp;
        process.push(in->ins[i]);
      }
    }
  }
  wires["1\'b0"]->m_value = BitVec(m_num_samples, 0);
  wires["1\'b1"]->m_value = BitVec(m_num_samples, 1);
  process.push(wires["1\'b0"]);
  process.push(wires["1\'b1"]);
  // cout << "input put into queue" << endl;
  // ofstream debug("debug.txt");
  // debug << "initial queue size: " << process.size() << endl;
  while (!process.empty()) {
    Component *now = process.front();
    // debug << now->name << endl;
    process.pop();
    BitVec outValue = now->m_value;
    // debug << "fanout num: " << now->fanouts.size() << endl;
    for (auto out : now->m_fanouts) {
      // debug << out->name << " " << out->fanins.size() << endl;
      out->m_setup_num++;
      if (out->m_type == NET || out->m_type == OUTPUT) {
        assert(out->m_fanins.size() == 1);
        out->m_value = outValue;
        process.push(out);
      } else if (out->m_type == GATE) {
        assert(now->m_fanins.size() <= 2);
        if (out->m_setup_num == out->m_fanins.size()) {
          if (out->m_fanins.size() == 2) {
            out->m_value =
                Simulator::calculate(out->m_gateType, out->m_fanins[0]->m_value,
                                     out->m_fanins[1]->m_value);
          } else {
            out->m_value = Simulator::calculate(out->m_gateType,
                                                out->m_fanins[0]->m_value);
          }
          process.push(out);
        }
      }
    }
    // debug << "queue size: " << process.size() << endl << endl;
    now->m_setup_num = 0;
  }
  // cout << "output done" << endl;
  for (Out *out : outputs) {
    string name = out->name;
    vector<BitVec> out_val;
    // cout << name << endl;
    for (int i = 0; i < m_num_samples; i++) {
      BitVec tmp;
      if (out->width == -1) {
        // cout << out->outs.size() << endl;
        // cout << out->outs[0]->value.size() << endl;
        // cout << out->outs[0]->value[i] << flush;
        tmp.push_back(out->outs[0]->m_value[i]);
      } else {
        for (int j = out->width - 1; j >= 0; j--) {
          tmp.push_back(out->outs[j]->m_value[i]);
        }
      }
      out_val.push_back(tmp);
    }
    output_testcase[name] = out_val;
  }
  if (DEBUG_MSG) cout << "output generated" << endl;
  return;
};

// TODO: use better randomizer
void Simulator::generate_input(int num, string fixed, long long val) {
  if (DEBUG_MSG) {
    cout << "start generating inputs...\n";
  }
  m_num_samples = num;
  input_testcase.clear();
  srand(time(NULL));

  for (auto [name, width] : input_infos) {
    vector<BitVec> vecs;
    for (int i = 0; i < num; i++) {
      BitVec vec;
      if (width == -1) {
        if (name == fixed) {
          vec.push_back(val);
        } else {
          vec.push_back(rand() % 2);
        }
      } else {
        if (name == fixed) {
          long long tmpv = val;
          for (int j = 0; j < width; j++) {
            vec.push_back(tmpv & 1);
            tmpv >>= 1;
          }
          reverse(vec.begin(), vec.end());
        } else {
          for (int j = 0; j < width; j++) {
            vec.push_back(rand() % 2);
          }
        }
      }
      vecs.push_back(vec);
    }
    input_testcase[name] = vecs;
  }
  if (DEBUG_MSG) {
    cout << "inputs generated\n";
  }
}

// BitVec Simulator::calculate(GateType gateType, BitVec v1, BitVec v2) {
//   BitVec ret;
//   switch (gateType) {
//     case AND:
//       for (int i = 0; i < v1.size(); i++) ret.push_back(v1[i] && v2[i]);
//       return ret;
//     case OR:
//       for (int i = 0; i < v1.size(); i++) ret.push_back(v1[i] || v2[i]);
//       return ret;
//     case NAND:
//       for (int i = 0; i < v1.size(); i++) ret.push_back(!(v1[i] && v2[i]));
//       return ret;
//     case NOR:
//       for (int i = 0; i < v1.size(); i++) ret.push_back(!(v1[i] || v2[i]));
//       return ret;
//     case NOT:
//       for (int i = 0; i < v1.size(); i++) ret.push_back(!v1[i]);
//       return ret;
//     case BUF:
//       for (int i = 0; i < v1.size(); i++) ret.push_back(v1[i]);
//       return ret;
//     case XOR:
//       for (int i = 0; i < v1.size(); i++) ret.push_back(v1[i] ^ v2[i]);
//       return ret;
//     case XNOR:
//       for (int i = 0; i < v1.size(); i++) ret.push_back(!(v1[i] ^ v2[i]));
//       return ret;
//     default:
//       cerr << "not any gate type\n";
//       break;
//   }
//   return {};
// }

void Simulator::simulate(int num_samples, string fixed, long long val) {
  generate_input(num_samples, fixed, val);
  generate_output();
}
