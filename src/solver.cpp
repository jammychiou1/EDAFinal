#include "solver.h"

#include <cassert>

using namespace std;
using namespace flint;

string Solver::format_term(const TermDesc &term, BigInt coeff) {
  assert(coeff != 0);
  if (term.empty()) {
    return coeff.to_string();
  }
  stringstream term_ss;
  bool first = true;
  if (coeff != 1) {
    term_ss << coeff;
    first = false;
  }
  for (int input_idx : term) {
    Word word = m_input_infos.get_word(input_idx);

    if (first) {
      term_ss << word.name;
      first = false;
    } else {
      term_ss << " * " << word.name;
    }
  }
  return term_ss.str();
}

string Solver::format_formula(const Formula &formula) {
  stringstream formula_ss;
  bool first = true;
  for (const auto &[term, coeff] : formula) {
    if (coeff != 0) {
      if (first) {
        formula_ss << format_term(term, coeff);
        first = false;
      } else {
        if (coeff < 0) {
          formula_ss << " - " << format_term(term, BigInt(-coeff));
        } else {
          formula_ss << " + " << format_term(term, coeff);
        }
      }
    }
  }
  if (first) {
    return "0";
  }
  return formula_ss.str();
}

vector<TermDesc> n_m_simple_terms(int max_sum, int max_single, int num_inputs) {
  vector<TermDesc> result;
  TermDesc term_now;

  function<void(int, int)> recursion = [&](int depth, int prefix_sum) -> void {
    if (depth >= num_inputs) {
      result.push_back(term_now);
      return;
    }
    // if (term_now.size() >= max_sum) {
    //   result.push_back(term_now);
    //   return;
    // }
    int mn = min(max_sum - prefix_sum, max_single);
    for (int i = 0; i <= mn; i++) {
      recursion(depth + 1, prefix_sum + i);
      term_now.push_back(depth);
    }
    for (int i = 0; i <= mn; i++) {
      term_now.pop_back();
    }
  };

  recursion(0, 0);
  return result;
}

BigInt calc_term(const vector<BigInt> &input, const TermDesc &term,
                 BigInt mask) {
  BigInt res(1);
  for (int a : term) {
    res = (res * input[a]) & mask;
  }
  return res;
}

vector<BigInt> gen_row(const vector<BigInt> &inputs,
                       const vector<TermDesc> &terms, BigInt mask) {
  vector<BigInt> res;
  for (const TermDesc &term : terms) {
    res.push_back(calc_term(inputs, term, mask));
  }
  return res;
}

Solver::Solver(const map<string, int> &input_widths,
               const map<string, int> &output_widths) {
  for (auto [input_name, width] : input_widths) {
    m_input_infos.add_word(input_name, width);
  }
  for (auto [output_name, width] : output_widths) {
    m_output_infos.add_word(output_name, width);
  }
}

void Solver::load_data(int num_samples,
                       const map<string, vector<BigInt>> &inputs,
                       const map<string, vector<BigInt>> &outputs) {
  int num_inputs = m_input_infos.get_num_words();
  int num_outputs = m_output_infos.get_num_words();
  Sample empty_sample{vector<BigInt>(num_inputs), vector<BigInt>(num_outputs)};
  m_samples = vector<Sample>(num_samples, empty_sample);

  for (Word word : m_input_infos) {
    string name = word.name;
    int idx = word.idx;

    for (int t = 0; t < num_samples; t++) {
      m_samples[t].inputs[idx] = inputs.at(name)[t];
    }
  }

  for (Word word : m_output_infos) {
    string name = word.name;
    int idx = word.idx;

    for (int t = 0; t < num_samples; t++) {
      m_samples[t].outputs[idx] = outputs.at(name)[t];
    }
  }
}

BigInt width_to_mask(int width) {
  if (width == -1) {
    return BigInt(1);
  }
  return BigInt((BigInt(1) << width) - 1);
}

optional<Formula> Solver::solve_output(string name) {
  Word word = m_output_infos.get_word(name);
  int output_idx = word.idx;
  BigInt mask = width_to_mask(word.width);

  // cout << mask << '\n';

  vector<TermDesc> terms =
      n_m_simple_terms(3, 3, m_input_infos.get_num_words());

  // for (TermDesc term : terms) {
  //   cout << m_print_term(term) << '\n';
  // }
  // cout << endl;

  int cols = terms.size();
  int rows = m_samples.size();
  Mat A(rows, cols);
  Mat b(rows, 1);
  for (int i = 0; i < rows; i++) {
    vector<BigInt> row_inputs = gen_row(m_samples[i].inputs, terms, mask);
    BigInt row_output = m_samples[i].outputs[output_idx];
    for (int j = 0; j < cols; j++) {
      A.at(i, j) = row_inputs[j];
    }
    b.at(i, 0) = row_output;
  }

  // A.print_pretty();
  // cout << '\n';
  // b.print_pretty();
  // cout << '\n';

  auto coeffs = short_solution(A, b, mask);
  if (!coeffs) {
    cout << "No solution\n";
    return nullopt;
  }

  cout << "Solved\n";
  // coeffs->print_pretty();
  // cout << '\n';
  Formula formula;
  for (int i = 0; i < cols; i++) {
    if (coeffs->at(i, 0) != 0) {
      formula.emplace_back(terms[i], coeffs->at(i, 0));
    }
  }
  return formula;
}
