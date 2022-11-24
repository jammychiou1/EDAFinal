#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include "parser.h"
#include <bits/stdc++.h>

using namespace std;

class Simulator : public Parser {

public:
  Simulator() {
    wires["1\'b0"] = new Based("1\'b0", NET);
    wires["1\'b1"] = new Based("1\'b1", NET);
  };
  ~Simulator(){};

  void generate_output(map<string, vector<bitvec>> &);
  void generate_input(string fixed = "", int val = -1, int num = 1000);
  void read(const string &);
  pair<string, int> find_smallest_input();
  vector<pair<string, int>> input_info;
  vector<pair<string, int>> output_info;
  map<string, vector<bitvec>> input_testcase;
  map<string, vector<bitvec>> output_testcase;

private:
  bitvec calculate(GateType gateType, bitvec v1, bitvec v2 = {});
  void process_wire(string line);
  void print_val(string name, bitvec);
};

#endif
