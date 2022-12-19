#include "pcr.h"
#include "converter.h"
#include "solver.h"
#include "parser.h"

#include <iostream>

using namespace std;

const int n_samples = 1000;

BigInt bitvec_to_bigint(const BitVec &vec) {
  BigInt bigInt(0);
  for (int i = 0; i < vec.size(); i++) {
    bigInt *= 2;
    if (vec[i]) {
      bigInt += 1;
    }
  }
  return bigInt;
}

bool pcr_case(string output_name, int output_width, string control_name, int control_width, Circuit &circuit, Converter &converter) {
  if (control_width > 15) {
    cout << "Too many cases\n";
    return false;
  }

  map<string, int> input_widths = circuit.get_input_widths();
  input_widths.erase(control_name);

  Solver case_solver(input_widths, {{output_name, output_width}});

  map<long long, string> expressions;
  long long n_cases = control_width == -1 ? 1 : (1ll << control_width);
  for (long long val = 0; val < n_cases; val++) {
    cout << control_name << ": " << val << '\n';
    auto [inputs, outputs] = circuit.generate_simulation(n_samples, {{control_name, val}});
    case_solver.load_data(n_samples, inputs, outputs);
    auto case_result = case_solver.solve_output(output_name);
    if (!case_result) {
      return false;
    }
    expressions[val] = case_solver.format_formula(case_result.value());
  }
  converter.write_output_cases(output_name, control_name, expressions);
  return true;
}

bool pcr_cases(string output_name, int output_width, Circuit &circuit, Converter &converter) {
  for (auto [control_name, control_width] : circuit.get_input_widths()) {
    cout << "Testing with control input " << control_name << '\n';

    if (pcr_case(output_name, output_width, control_name, control_width, circuit, converter)) {
      return true;
    }
  }
  return false;
}

bool pcr(string output_name, int output_width, Circuit &circuit, Converter &converter) {
  Solver solver(circuit.get_input_widths(), {{output_name, output_width}});
  auto [inputs, outputs] = circuit.generate_simulation(n_samples);
  solver.load_data(n_samples, inputs, outputs);
  auto result = solver.solve_output(output_name);
  if (!result) {
    return false;
  }
  converter.write_output(output_name, solver.format_formula(result.value()));
  return true;
}

bool polynomial_coefficient_recovering(string in_file, string out_file) {
  Parser parser;
  Circuit circuit = parser.parse(in_file);
  auto input_widths = circuit.get_input_widths();
  auto output_widths = circuit.get_output_widths();

  if (input_widths.size() > 10) {
    cout << "Too many inputs\n";
    return false;
  }

  Converter converter(out_file, input_widths, output_widths);

  converter.write_prologue();

  for (const auto &[output_name, width] : circuit.get_output_widths()) {
    cout << "Solving for " << output_name << ":\n";
    if (bool result = pcr(output_name, width, circuit, converter); result) {
      cout << "Solved by Polynomial Coefficient Recovering\n";
      continue;
    }
    if (bool result = pcr_cases(output_name, width, circuit, converter); result) {
      cout << "Solved by Polynomial Coefficient Recovering with Control Input\n";
      continue;
    }
    cout << "Failed\n";
    return false;
  }

  converter.write_epilogue();
  return true;
}
