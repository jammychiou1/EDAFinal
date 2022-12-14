#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "parser.h"
#include "solver.h"

// TODO: remove this file
class Simulator {
 public:
  void simulate(int num_samples, std::string fixed, long long val);

  std::map<std::string, std::vector<BitVec>> input_testcase;
  std::map<std::string, std::vector<BitVec>> output_testcase;

 private:
  void generate_output();
  void generate_input(int num = 1000, std::string fixed = "",
                      long long val = -1);
  // BitVec calculate(GateType gateType, BitVec v1, BitVec v2 = {});

  int m_num_samples;
};

#endif  // __SIMULATOR_H__
