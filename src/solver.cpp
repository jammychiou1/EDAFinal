#include "solver.h"
#include <cassert>
#include <flint/fmpz_matxx.h>
#include <iomanip>

using namespace std;
using namespace flint;

typedef fmpz_matxx Mat;

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

void add_c_row_i_to_j(Mat &A, int i, int j, BigInt c, BigInt mask, bool use_mask = true) {
  int cols = A.cols();
  for (int k = 0; k < cols; k++) {
    A.at(j, k) = A.at(j, k) + c * A.at(i, k);
    if (use_mask) {
      A.at(j, k) = A.at(j, k) & mask;
    }
  }
}

void add_c_col_i_to_j(Mat &A, int i, int j, BigInt c, BigInt mask, bool use_mask = true) {
  int rows = A.rows();
  for (int k = 0; k < rows; k++) {
    A.at(k, j) = (A.at(k, j) + c * A.at(k, i));
    if (use_mask) {
      A.at(k, j) = A.at(k, j) & mask;
    }
  }
}

void swap_row_i_j(Mat &A, int i, int j) {
  int cols = A.cols();
  for (int k = 0; k < cols; k++) {
    BigInt tmp(A.at(i, k));
    A.at(i, k) = A.at(j, k);
    A.at(j, k) = tmp;
  }
}

void swap_col_i_j(Mat &A, int i, int j) {
  int rows = A.rows();
  for (int k = 0; k < rows; k++) {
    BigInt tmp(A.at(k, i));
    A.at(k, i) = A.at(k, j);
    A.at(k, j) = tmp;
  }
}

bool elim_col(Mat &A, Mat &L, int now, BigInt mask) {
  int rows = A.rows();
  bool flag = false;
  for (int i = now + 1; i < rows; i++) {
    if (A.at(i, now) != 0) {
      flag = true;
      while (true) {
        BigInt c(A.at(i, now) / A.at(now, now));
        BigInt neg_c(negmod(c, mask + 1));
        add_c_row_i_to_j(A, now, i, neg_c, mask);
        add_c_row_i_to_j(L, now, i, neg_c, mask, false);
        if (A.at(i, now) == 0) {
          break;
        }
        swap_row_i_j(A, now, i);
        swap_row_i_j(L, now, i);
      }
    }
  }
  return flag;
}

bool elim_row(Mat &A, Mat &R, int now, BigInt mask) {
  int cols = A.cols();
  bool flag = false;
  for (int i = now + 1; i < cols; i++) {
    if (A.at(now, i) != 0) {
      flag = true;
      while (true) {
        BigInt c(A.at(now, i) / A.at(now, now));
        BigInt neg_c(mask + 1 - c);
        add_c_col_i_to_j(A, now, i, neg_c, mask);
        add_c_col_i_to_j(R, now, i, neg_c, mask, false);
        if (A.at(now, i) == 0) {
          break;
        }
        swap_col_i_j(A, now, i);
        swap_col_i_j(R, now, i);
      }
    }
  }
  return flag;
}

bool make_pivot(Mat &A, Mat &L, Mat &R, int now) {
  int rows = A.rows();
  int cols = A.cols();
  for (int i = now; i < rows; i++) {
    for (int j = now; j < cols; j++) {
      if (A.at(i, j) != 0) {
        swap_row_i_j(A, now, i);
        swap_row_i_j(L, now, i);
        swap_col_i_j(A, now, j);
        swap_col_i_j(R, now, j);
        return true;
      }
    }
  }
  return false;
}

// return {D, L, R}
// D = L A R
std::tuple<Mat, Mat, Mat> smith_normal_form(Mat A, BigInt mask) {
  int rows = A.rows();
  int cols = A.cols();
  int mn = min(rows, cols);
  Mat D(A);
  Mat L = fmpz_matxx::one(rows, rows);
  Mat R = fmpz_matxx::one(cols, cols);
  for (int now = 0; now < mn; now++) {
    if (!make_pivot(D, L, R, now)) {
      break;
    }
    while (true) {
      bool flag = false;
      flag |= elim_col(D, L, now, mask);
      flag |= elim_row(D, R, now, mask);
      if (!flag) {
        break;
      }
    }
  }
  return {D, L, R};
}

void lll_reduce(Mat &L) {
  fmpqxx delta(75, 100u), eta(51, 100u);
  fmpz_mat_lll_storjohann(L._mat(), delta._fmpq(), eta._fmpq());

  // L.print_pretty();
  // cout << '\n';
}

// Minkowski's first theorem:
// For Lattice L of dimension n, shortest nonzero vector in L has norm less than
// (about) sqrt(n) det(L)^(1/n).
//
// Property of LLL-reduced lattice:
// First basis vector returned in LLL is not much longer than shortest nonzero
// vector.
Mat closest_vector_embedded(Mat L, Mat y, BigInt mask) {
  int d = L.cols();

  int rounds = 100;

  Mat A(d + 1, d + 1 + rounds);

  for (int i = 0; i < d; i++) {
    for (int j = 0; j < d; j++) {
      A.at(i, j) = L.at(i, j);
    }
  }

  for (int i = 0; i < d; i++) {
    A.at(d, i) = y.at(i, 0);
  }

  // for (int i = 0; i < d; i++) {
  //   A.at(i, d + i) = 1;
  // }

  // BigInt M(mask.sqrt());
  BigInt weight(mask);
  BigInt weight_y(mask.sqrt());
  A.at(d, d) = weight_y;

  lll_reduce(L);
  // L.print_pretty();
  // cout << " is reduced L\n";

  for (int t = 0; t < rounds; t++) {
    // cout << "----------" << t << "----------\n";

    // A.print_pretty();
    // cout << " is A\n";

    lll_reduce(A);

    // A.print_pretty();
    // cout << " is reduced A\n";

    if (A.at(0, d).abs() == weight_y) {
      Mat ans(d, 1);
      if (A.at(0, d) == weight_y) {
        for (int j = 0; j < d; j++) {
          ans.at(j, 0) = A.at(0, j);
        }
      } else {
        for (int j = 0; j < d; j++) {
          ans.at(j, 0) = -A.at(0, j);
        }
      }
      return ans;
    }
    else {
      A.at(0, d + 1 + t) = weight;
    }
  }
  return y;
}

BigInt div_round(BigInt p, BigInt q) {
  if (p < 0) {
    return BigInt(-div_round(BigInt(-p), q));
  }
  q = q.abs();

  assert(p >= 0 && q > 0);

  BigInt rem(p % q);
  return (rem * 2 > q) ? BigInt(p / q + 1) : BigInt(p / q);
}

Mat closest_vector_babai(Mat L, Mat y, BigInt mask) {
  int d = L.cols();

  // L.print_pretty();
  // cout << " is L\n";

  lll_reduce(L);

  // L.print_pretty();
  // cout << " is L\n";

  L = L.transpose();

  // L.print_pretty();
  // cout << " is L\n";

  // y.print_pretty();
  // cout << " is y\n";

  auto res = solve(L, y);
  Mat x(res.get<1>());
  BigInt den(res.get<2>());

  // x.print_pretty();
  // cout << '\n';
  // den.print();
  // cout << '\n';

  for (int i = 0; i < d; i++) {
    x.at(i, 0) = div_round(BigInt(x.at(i, 0)), den);
  }

  // x.print_pretty();
  // cout << '\n';
  // (L * x).print_pretty();
  // cout << '\n';

  Mat yy(y - L * x);

  // yy.print_pretty();
  // cout << '\n';

  return yy;
}

optional<Mat> short_solution(Mat A, Mat b, BigInt mask) {
  auto [D, L, R] = smith_normal_form(A, mask);

  // L A R = D
  //
  // A x = b
  // L A x = L b
  // D y = D RR x = L A R RR x = L A x = L b = z
  //
  // A x = 0
  // L A x = 0
  // D y = D RR x = L A R RR x = L A x = 0

  Mat z(L * b);
  for (int i = 0; i < z.rows(); i++) {
    z.at(i, 0) = BigInt(z.at(i, 0) & mask);
  }

  // z.print_pretty();

  int rows = A.rows();
  int cols = A.cols();
  int mn = min(rows, cols);

  Mat y(cols, 1);
  Mat null_D(Mat::one(cols, cols));

  // D.print_pretty();
  // cout << '\n';
  // z.print_pretty();
  // cout << '\n';

  for (int i = 0; i < mn; i++) {
    BigInt d(D.at(i, i));
    BigInt zi(z.at(i, 0));
    // yi * d = zi (mod mask + 1)

    if (d == 0) {
      if (zi != 0) {
        return nullopt;
      }
      y.at(i, 0) = BigInt(0);
      null_D.at(i, i) = BigInt(1);
      continue;
    }

    BigInt gcd_d(gcd(d, mask + 1));
    BigInt gcd_zi(gcd(zi, mask + 1));
    if (gcd_d > gcd_zi) {
      return nullopt;
    }

    BigInt k_d(d / gcd_d);
    BigInt k_zi(zi / gcd_d);
    y.at(i, 0) = BigInt((k_zi * invmod(k_d, mask + 1)) & mask);

    null_D.at(i, i) = BigInt((mask + 1) / gcd_d);
  }

  for (int i = mn; i < cols; i++) {
    if (z.at(i, 0) != 0) {
      return nullopt;
    }
  }

  Mat x0(R * y);
  for (int i = 0; i < cols; i++) {
    x0.at(i, 0) = x0.at(i, 0) & mask;
  }

  Mat lat((R * null_D).transpose());

  // for (int i = 0; i < cols; i++) {
  //   for (int j = 0; j < cols; j++) {
  //     cout << setw(5) << R.at(i, j);
  //   }
  //   cout << '\n';
  // }

  // x0.print_pretty();
  // cout << '\n';
  // R.print_pretty();
  // cout << '\n';
  // null_D.print_pretty();
  // cout << '\n';
  // lat.print_pretty();
  // cout << '\n';

  Mat ans(closest_vector_embedded(lat, x0, mask));
  return ans;

  // Mat ans_a(closest_vector_embedded(lat, x0, mask));
  // Mat ans_b(closest_vector_babai(lat, x0, mask));
  // ans_a.print_pretty();
  // ans_b.print_pretty();
  // assert(ans_a == ans_b);
  // return ans_a;
}

BigInt to_mask(int bit_width) {
  BigInt ans;
  ans.setbit(bit_width);
  return BigInt(ans - 1);
}

string Solver::m_print_term(TermDesc term, BigInt coeff = BigInt(1)) {
  assert(coeff != 0);
  if (term.empty()) {
    return coeff.to_string();
  }
  stringstream term_ss;
  bool tail = false;
  if (coeff != 1) {
    term_ss << coeff;
    tail = true;
  }
  for (int l = 0, r = 0; l < term.size(); l = r) {
    while (r < term.size() && term[r] == term[l]) {
      r++;
    }

    stringstream power;
    power << m_input_names[term[l]];
    if (r - l != 1) {
      power << " ** " << r - l;
    }

    if (tail) {
      term_ss << " * " << power.str();
    } else {
      term_ss << power.str();
      tail = true;
    }
  }
  return term_ss.str();
}

string Solver::m_print_terms(vector<TermDesc> terms, vector<BigInt> coeffs) {
  stringstream terms_ss;
  bool tail = false;
  for (int i = 0; i < terms.size(); i++) {
    if (coeffs[i] != 0) {
      if (tail) {
        if (coeffs[i] < 0) {
          terms_ss << " - " << m_print_term(terms[i], BigInt(-coeffs[i]));
        } else {
          terms_ss << " + " << m_print_term(terms[i], coeffs[i]);
        }
      } else {
        terms_ss << m_print_term(terms[i], coeffs[i]);
        tail = true;
      }
    }
  }
  return terms_ss.str();
}

void Solver::def_input(string name, int bit_width) {
  InputDesc desc{m_input_size(), to_mask(bit_width)};
  m_input_descs[name] = desc;
  m_input_names.push_back(name);
}

void Solver::def_output(string name, int bit_width) {
  OutputDesc desc{m_output_size(), to_mask(bit_width)};
  m_output_descs[name] = desc;
  m_output_names.push_back(name);
}

void Solver::add_sample(map<string, BigInt> inputs,
                        map<string, BigInt> outputs) {
  Sample sample{vector<BigInt>(m_input_size()),
                vector<BigInt>(m_output_size())};
  for (auto [name, desc] : m_input_descs) {
    sample.inputs[desc.loc] = inputs[name];
  }
  for (auto [name, desc] : m_output_descs) {
    sample.outputs[desc.loc] = outputs[name];
  }
  m_samples.push_back(sample);
}

void Solver::solve_output(std::string name) {
  int loc = m_output_descs[name].loc;
  BigInt mask = m_output_descs[name].mask;

  vector<TermDesc> terms = n_m_simple_terms(3, 3, m_input_size());

  for (TermDesc term : terms) {
    cout << m_print_term(term) << '\n';
  }
  cout << '\n';

  int cols = terms.size();
  int rows = m_samples.size();
  Mat A(rows, cols);
  Mat b(rows, 1);
  for (int i = 0; i < rows; i++) {
    vector<BigInt> row_inputs = gen_row(m_samples[i].inputs, terms, mask);
    BigInt row_output = m_samples[i].outputs[loc];
    for (int j = 0; j < cols; j++) {
      A.at(i, j) = row_inputs[j];
    }
    b.at(i, 0) = row_output;
  }

  auto coeffs = short_solution(A, b, mask);
  if (coeffs) {
    cout << "Solved\n";
    coeffs->print_pretty();
    cout << '\n';
  } else {
    cout << "No solution\n";
  }
}

void Solver::solve() {}
