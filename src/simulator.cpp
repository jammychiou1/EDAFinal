#include "simulator.h"
#define DEBUG_MSG false
void Simulator::generate_output(map<string, vector<bitvec>> &testcase) {
  if (DEBUG_MSG)
    cout << "start generate output..." << endl;
  // cout << "gate count: " << gate_cnt << endl;
  output_testcase.clear();
  int testcase_size = (int)testcase.begin()->second.size();
  queue<Based *> process;
  for (auto it = testcase.begin(); it != testcase.end(); it++) {
    for (In *in : inputs) {
      if (in->name == it->first) {
        if (in->width == -1) {
          // from [[0], [1], [1]] to [0, 1, 1]
          bitvec tmp;
          for (size_t i = 0; i < it->second.size(); i++)
            tmp.push_back(it->second[i][0]);
          in->ins[0]->value = tmp;
          process.push(in->ins[0]);
        } else {
          // from [[0, 1, 1], [1, 0, 1], [1, 0, 0]]
          // to [0, 1, 1], [1, 0, 0], [1, 1, 0]
          for (int i = 0; i < in->width; i++) {
            bitvec tmp;
            for (size_t j = 0; j < it->second.size(); j++) {
              tmp.push_back(it->second[j][in->width - i - 1]);
            }
            in->ins[i]->value = tmp;
            process.push(in->ins[i]);
          }
        }
        break;
      }
    }
  }
  wires["1\'b0"]->value = bitvec(testcase_size, 0);
  wires["1\'b1"]->value = bitvec(testcase_size, 1);
  process.push(wires["1\'b0"]);
  process.push(wires["1\'b1"]);
  // cout << "input put into queue" << endl;
  // ofstream debug("debug.txt");
  // debug << "initial queue size: " << process.size() << endl;
  while (!process.empty()) {
    Based *now = process.front();
    // debug << now->name << endl;
    process.pop();
    bitvec outValue = now->value;
    // debug << "fanout num: " << now->fanouts.size() << endl;
    for (auto out : now->fanouts) {
      // debug << out->name << " " << out->fanins.size() << endl;
      out->setup_num++;
      if (out->type == NET || out->type == OUTPUT) {
        assert(out->fanins.size() == 1);
        out->value = outValue;
        process.push(out);
      } else if (out->type == GATE) {
        assert(now->fanins.size() <= 2);
        if (out->setup_num == out->fanins.size()) {
          if (out->fanins.size() == 2) {
            out->value = Simulator::calculate(
                out->gateType, out->fanins[0]->value, out->fanins[1]->value);
          } else {
            out->value =
                Simulator::calculate(out->gateType, out->fanins[0]->value);
          }
          process.push(out);
        }
      }
    }
    // debug << "queue size: " << process.size() << endl << endl;
    now->setup_num = 0;
  }
  // cout << "output done" << endl;
  for (Out *out : outputs) {
    string name = out->name;
    vector<bitvec> out_val;
    // cout << name << endl;
    for (int i = 0; i < testcase_size; i++) {
      bitvec tmp;
      if (out->width == -1) {
        // cout << out->outs.size() << endl;
        // cout << out->outs[0]->value.size() << endl;
        // cout << out->outs[0]->value[i] << flush;
        tmp.push_back(out->outs[0]->value[i]);
      } else {
        for (int j = out->width - 1; j >= 0; j--) {
          tmp.push_back(out->outs[j]->value[i]);
        }
      }
      out_val.push_back(tmp);
    }
    output_testcase[name] = out_val;
  }
  // cout << "process done" << endl;
  // for (auto it = output_testcase.begin(); it != output_testcase.end(); it++)
  // {
  //     cout << it->first << endl;
  //     for (int i = 0; i < it->second.size(); i++) {
  //         for (int j = 0; j < it->second[0].size(); j++) {
  //             cout << it->second[i][j];
  //         }
  //         cout << endl;
  //     }
  // }
  if (DEBUG_MSG)
    cout << "output generated" << endl;
  return;
};

bitvec Simulator::calculate(GateType gateType, bitvec v1, bitvec v2) {
  bitvec ret;
  switch (gateType) {
  case AND:
    for (size_t i = 0; i < v1.size(); i++)
      ret.push_back(v1[i] & v2[i]);
    return ret;
  case OR:
    for (size_t i = 0; i < v1.size(); i++)
      ret.push_back(v1[i] | v2[i]);
    return ret;
  case NAND:
    for (size_t i = 0; i < v1.size(); i++)
      ret.push_back(!(v1[i] & v2[i]));
    return ret;
  case NOR:
    for (size_t i = 0; i < v1.size(); i++)
      ret.push_back(!(v1[i] | v2[i]));
    return ret;
  case NOT:
    for (size_t i = 0; i < v1.size(); i++)
      ret.push_back(!v1[i]);
    return ret;
  case BUF:
    for (size_t i = 0; i < v1.size(); i++)
      ret.push_back(v1[i]);
    return ret;
  case XOR:
    for (size_t i = 0; i < v1.size(); i++)
      ret.push_back(v1[i] ^ v2[i]);
    return ret;
  case XNOR:
    for (size_t i = 0; i < v1.size(); i++)
      ret.push_back(!(v1[i] ^ v2[i]));
    return ret;
  default:
    cerr << "not any gate type\n";
    break;
  }
  return {};
}

void Simulator::process_wire(string line) {
  line = trim(line.substr(4));
  pair<int, string> result = process_word_desc(line);
  line = result.second;
  int width = result.first;

  line.push_back(',');
  string now;
  if (width == -1) {
    for (size_t i = 0; i < line.size(); i++) {
      if (line[i] == ',') {
        const string name = trim(now);
        if (wires.find(name) == wires.end()) {
          wires[name] = new Based(name, NET);
        }
        now = "";
        continue;
      }
      now.push_back(line[i]);
    }
  } else {
    for (size_t i = 0; i < line.size(); i++) {
      if (line[i] == ',') {
        now = trim(now);
        for (int j = 0; j < width; j++) {
          const string name = now + "[" + to_string(j) + "]";
          if (wires.find(name) == wires.end()) {
            wires[name] = new Based(name, NET);
          }
        }
        now = "";
        continue;
      }
      now.push_back(line[i]);
    }
  }
}

void Simulator::print_val(string name, bitvec data) {
  cout << name << endl;
  for (size_t i = 0; i < data.size(); i++) {
    cout << data[i] << " ";
  }
  cout << endl;
}

void Simulator::generate_input(int num, string fixed, long long val) {
  if (DEBUG_MSG)
    cout << "start generating inputs..." << endl;
  input_testcase.clear();
  srand(time(NULL));

  for (In *in : inputs) {
    vector<bitvec> ins;
    for (int i = 0; i < num; i++) {
      bitvec tmp;
      if (in->width == -1) {
        if (in->name == fixed) {
          tmp.push_back((bool)val);
        } else {
          tmp.push_back(rand() % 2);
        }
      } else {
        if (in->name == fixed) {
          int tmp_val = val;
          for (int j = 0; j < in->width; j++) {
            tmp.push_back((bool)(tmp_val % 2));
            tmp_val = tmp_val >> 1;
          }
          reverse(tmp.begin(), tmp.end());
        } else {
          for (int j = 0; j < in->width; j++) {
            tmp.push_back(rand() % 2);
          }
        }
      }
      ins.push_back(tmp);
    }
    input_testcase[in->name] = ins;
  }
  if (DEBUG_MSG)
    cout << "inputs generated" << endl;
}

void Simulator::read(const string &path) {
  Parser::read(path);
  input_info.clear();
  output_info.clear();
  for (auto &in : inputs) {
    input_info.push_back(make_pair(in->name, in->width));
  }
  for (auto &out : outputs) {
    output_info.push_back(make_pair(out->name, out->width));
  }
}

pair<string, int> Simulator::find_smallest_input() {
  int minWidth = INT_MAX;
  string minInput = "";
  for (const auto &in : inputs) {
    if (in->width < minWidth) {
      minWidth = in->width;
      minInput = in->name;
    }
  }
  return make_pair(minInput, minWidth);
}
