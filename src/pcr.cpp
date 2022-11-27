#include "pcr.h"
#include "converter.h"
#include "simulator.h"
#include "solver.h"

#include <iostream>

using std::string;

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

bool polynomial_coefficient_recovering(string in_file, string out_file) {
  Simulator simulator;
  // argc < 2 ? simulator.read("release/test01/top_primitive.v") :
  // simulator.read(argv[1]);
  simulator.read(in_file);
  // cout << "generating inputs..." << endl;

  int n_samples = 1000;
  simulator.generate_input(n_samples);
  cout << "input generated\nstart generating outputs..." << endl;
  simulator.generate_output(simulator.input_testcase);
  cout << "output generated\nstart solving..." << endl;

  // auto result = solve(simulator.input_testcase, simulator.output_testcase);

  Solver solver;
  for (auto [name, samples] : simulator.input_testcase) {
    solver.def_input(name, samples[0].size());
  }
  for (auto [name, samples] : simulator.output_testcase) {
    solver.def_output(name, samples[0].size());
  }
  for (int t = 0; t < n_samples; t++) {
    map<string, BigInt> inputs;
    map<string, BigInt> outputs;
    for (auto [name, samples] : simulator.input_testcase) {
      inputs[name] = bitvec_to_bigint(samples[t]);
    }
    for (auto [name, samples] : simulator.output_testcase) {
      outputs[name] = bitvec_to_bigint(samples[t]);
    }
    solver.add_sample(inputs, outputs);
  }

  bool result = solver.solve();
  if (result) {
    cout << "solved by Polynomial Coefficient Recovering" << endl;
  }
  // Converter converter(simulator.input_info, simulator.output_info);
}
