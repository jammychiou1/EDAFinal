#ifndef SOLVER_H
#define SOLVER_H

#include <functional>
#include <map>
#include <optional>
#include <utility>
#include <vector>

#include <flint/fmpz_matxx.h>
#include <flint/fmpzxx.h>

typedef flint::fmpzxx BigInt;
typedef std::vector<int> term_desc;

std::optional<flint::fmpz_matxx>
short_solution(flint::fmpz_matxx A, flint::fmpz_matxx b, BigInt mask);

class Solver {
private:
  struct InputDesc {
    int loc;
    BigInt mask;
  };
  struct OutputDesc {
    int loc;
    BigInt mask;
  };
  struct Sample {
    std::vector<BigInt> inputs;
    std::vector<BigInt> outputs;
  };

  int m_num_inputs = 0;
  int m_num_outputs = 0;
  std::map<std::string, InputDesc> m_input_descs;
  std::map<std::string, OutputDesc> m_output_descs;
  std::vector<Sample> m_samples;

public:
  void def_input(std::string name, int bit_width);
  void def_output(std::string name, int bit_width);
  void add_sample(std::map<std::string, BigInt> inputs,
                  std::map<std::string, BigInt> outputs);
  void solve_output(std::string name);
  void solve();
};

#endif
