#include "math_util.h"

#include <cassert>

using namespace flint;
using namespace std;

void add_c_row_i_to_j(Mat &A, int i, int j, BigInt c, BigInt mask,
                      bool use_mask) {
  int cols = A.cols();
  for (int k = 0; k < cols; k++) {
    A.at(j, k) = A.at(j, k) + c * A.at(i, k);
    if (use_mask) {
      A.at(j, k) = A.at(j, k) & mask;
    }
  }
}

void add_c_col_i_to_j(Mat &A, int i, int j, BigInt c, BigInt mask,
                      bool use_mask) {
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
std::array<Mat, 3> smith_normal_form(Mat A, BigInt mask) {
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
  // L.print_pretty();
  // cout << '\n';

  fmpqxx delta(99, 100u), eta(51, 100u);
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

  int rounds = 10;

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
    } else {
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

  // b.print_pretty();
  // cout << '\n';
  // z.print_pretty();
  // cout << '\n';

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

  for (int i = mn; i < rows; i++) {
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

  Mat solution(closest_vector_embedded(lat, x0, mask));
  return solution;

  // Mat ans_a(closest_vector_embedded(lat, x0, mask));
  // Mat ans_b(closest_vector_babai(lat, x0, mask));
  // ans_a.print_pretty();
  // ans_b.print_pretty();
  // assert(ans_a == ans_b);
  // return ans_a;
}

BigInt to_mask(int bit_width) {
  BigInt mask;
  mask.setbit(bit_width);
  return BigInt(mask - 1);
}
