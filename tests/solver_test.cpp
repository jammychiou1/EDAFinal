#include "solver.h"
#include <bits/stdc++.h>
#include <flint/fmpz_matxx.h>
#include <flint/fmpzxx.h>
using namespace std;

typedef flint::fmpzxx BigInt;
typedef flint::fmpz_matxx Mat;
typedef flint::frandxx RandState;

int main() {
  RandState state;

  int n = 100;
  Mat A(n, 20), y(n, 1);
  BigInt M(512);
  BigInt mask(M - 1);

  Solver solver;

  solver.def_input("a", 9);
  solver.def_input("b", 9);
  solver.def_input("c", 9);
  solver.def_output("o1", 9);

  for (int i = 0; i < n; i++) {
    BigInt a = BigInt::randm(state, M);
    BigInt b = BigInt::randm(state, M);
    BigInt c = BigInt::randm(state, M);
    BigInt o1 = BigInt((69 + 2 * a + 4 * b + (M - 1) * c + 3 * a * b +
                        a * a * (M - 4) + 9 * (b * c) + (M - 5) * a * a * c) &
                       mask);
    solver.add_sample(
        {
            {"a", a},
            {"b", b},
            {"c", c},
        },
        {{"o1", o1}});
  }

  solver.solve_output("o1");
}
