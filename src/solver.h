#ifndef SOLVER_H
#define SOLVER_H

#include <functional>
#include <map>
#include <optional>
#include <utility>
#include <vector>

#include <flint/fmpzxx.h>
#include <flint/fmpz_matxx.h>

typedef flint::fmpzxx BigInt;
typedef std::vector<int> term_desc;

// typedef std::function<std::vector<BigInt>(std::vector<BigInt>)>
//     calc_row_fn_t;
// calc_row_fn_t calc_row_fn(int sum, int single);

std::tuple<flint::fmpz_matxx, flint::fmpz_matxx, flint::fmpz_matxx> smith_normal_form(flint::fmpz_matxx A, BigInt mask);
std::optional<flint::fmpz_matxx> special_solution(flint::fmpz_matxx A, flint::fmpz_matxx b, BigInt mask);

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