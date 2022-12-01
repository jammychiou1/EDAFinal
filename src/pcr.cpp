#include "pcr.h"
#include "converter.h"
#include "simulator.h"
#include "solver.h"

#include <iostream>

using std::string;

const int n_samples = 1000;

BigInt bitvec_to_bigint(bitvec v) {
  BigInt ans(0);
  for (int i = 0; i < v.size(); i++) {
    ans *= 2;
    if (v[i]) {
      ans += 1;
    }
  }
  return ans;
}

Solver make_solver(Simulator &simulator, string control_input = "") {
  Solver solver;

  for (auto [name, samples] : simulator.input_testcase) {
    if (name != control_input) {
      solver.def_input(name, samples[0].size());
    }
  }
  for (auto [name, samples] : simulator.output_testcase) {
    solver.def_output(name, samples[0].size());
  }
  for (int t = 0; t < n_samples; t++) {
    map<string, BigInt> inputs;
    map<string, BigInt> outputs;
    for (auto [name, samples] : simulator.input_testcase) {
      if (name != control_input) {
        inputs[name] = bitvec_to_bigint(samples[t]);
      }
    }
    for (auto [name, samples] : simulator.output_testcase) {
      outputs[name] = bitvec_to_bigint(samples[t]);
    }
    solver.add_sample(inputs, outputs);
  }
  return solver;
}

bool pcr_case(Simulator &simulator, string name, int width) {
  if (width > 15) {
    cout << "Too many cases\n";
    return false;
  }

  long long sz = width == -1 ? 1 : (1ll << width);
  for (long long i = 0; i < sz; i++) {
    cout << name << ": " << i << '\n';
    simulator.generate_input(n_samples, name, i);
    simulator.generate_output(simulator.input_testcase);
    Solver case_solver = make_solver(simulator, name);
    bool case_result = case_solver.solve();
    if (!case_result) {
      return false;
    }
    // converter.convert_control(output, sep_result.second,
    // possibly_control.first,
    //                           i);
  }
  return true;
}

bool pcr_cases(Simulator &simulator) {
  for (auto in : simulator.getIns()) {
    string name = in->name;
    int width = in->width;
    cout << "Testing with control input " << name << '\n';

    if (pcr_case(simulator, name, width)) {
      return true;
    }
  }
  return false;
}

bool pcr(Simulator &simulator) {
  simulator.generate_input(n_samples);
  cout << "Input generated\nStart generating outputs...\n";
  simulator.generate_output(simulator.input_testcase);
  cout << "Output generated\nStart solving...\n";

  Solver solver = make_solver(simulator);

  return solver.solve();
}

bool polynomial_coefficient_recovering(string in_file, string out_file) {
  Simulator simulator;
  // argc < 2 ? simulator.read("release/test01/top_primitive.v") :
  // simulator.read(argv[1]);
  simulator.read(in_file);
  // cout << "generating inputs..." << endl;

  if (pcr(simulator)) {
    cout << "Solved by Polynomial Coefficient Recovering\n";
    // Converter converter(simulator.input_info, simulator.output_info);
    return true;
  }
  if (pcr_cases(simulator)) {
    cout << "Solved by Polynomial Coefficient Recovering with Control Input\n";
    return true;
  }
  return false;
}
