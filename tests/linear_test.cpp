#include <bits/stdc++.h>
#include <flint/fmpz_matxx.h>
#include <flint/fmpzxx.h>
#include "solver.h"
using namespace std;

typedef flint::fmpzxx BigInt;
typedef flint::fmpz_matxx Mat;

int main() {
  int n = 15;
  Mat A(n, 3), y(n, 1);
  int M = 16;
  int mask = M - 1;
  for (int i = 0; i < n; i++) {
    int a = rand() % M;
    int b = rand() % M;
    int c = rand() % M;
    int o1 = (2 * a + 4 * b + mask * c) & mask;
    A.at(i, 0) = a;
    A.at(i, 1) = b;
    A.at(i, 2) = c;
    y.at(i, 0) = o1;
  }

  A.print_pretty();
  cout << '\n';
  y.print_pretty();
  cout << '\n';
  cout << '\n';

  optional<Mat> s = special_solution(A, y, BigInt(mask));
  s->print_pretty();

  // L A R = D
  // auto [D, L, R] = smith_normal_form(A, BigInt(mask));

  // D.print_pretty();
  // cout << '\n';
  // L.print_pretty();
  // cout << '\n';
  // R.print_pretty();
  // cout << '\n';


  // L A R = D
  // A x = b
  // L A x = L b
  // D RR x = L A R RR x = L b

  // Mat test(L * A * R);
  // for (int i = 0; i < test.rows(); i++) {
  //   for (int j = 0; j < test.cols(); j++) {
  //     test.at(i, j) = BigInt(test.at(i, j) & BigInt(mask));
  //     BigInt(test.at(i, j) & BigInt(mask)).print();
  //   }
  // }

  // test.print_pretty();
  // cout << '\n';

  // A.set_rref_mod(BigInt(M));
  // A.print_pretty();
  // cout << '\n';

  // Mat C(n, 3 + n);
  // for (int i = 0; i < n; i++) {
  //   for (int j = 0; j < 3; j++) {
  //     C.at(i, j) = A.at(i, j);
  //   }
  //   C.at(i, 3 + i) = M;
  // }
  // C.print_pretty();
  // cout << '\n';
  // Mat D(n, 3 + n);
  // int m;
  // flint::ltupleref(m, D);
  // auto res = flint::nullspace(C);
  // res.get<1>().print_pretty();
  // cout << '\n';
}
