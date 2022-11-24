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
typedef std::vector<int> TermDesc;

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

  std::map<std::string, InputDesc> m_input_descs;
  std::map<std::string, OutputDesc> m_output_descs;
  std::vector<std::string> m_input_names;
  std::vector<std::string> m_output_names;
  std::vector<Sample> m_samples;

  int m_input_size() { return m_input_names.size(); }

  int m_output_size() { return m_output_names.size(); }

  std::string m_print_term(TermDesc term, BigInt coeff);
  std::string m_print_terms(std::vector<TermDesc> terms,
                            std::vector<BigInt> coeffs);

public:
  void def_input(std::string name, int bit_width);
  void def_output(std::string name, int bit_width);
  void add_sample(std::map<std::string, BigInt> inputs,
                  std::map<std::string, BigInt> outputs);
  void solve_output(std::string name);
  void solve();
};

#endif
