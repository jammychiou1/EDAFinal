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

  Solver solver({{"a", 9}, {"b", 9}, {"c", 9}}, {{"o1", 9}});

  vector<BigInt> as, bs, cs, o1s;
  for (int i = 0; i < n; i++) {
    BigInt a = BigInt::randm(state, M);
    BigInt b = BigInt::randm(state, M);
    BigInt c = BigInt::randm(state, M);
    BigInt o1 = BigInt((69 + 2 * a + 4 * b + (M - 1) * c + 3 * a * b +
                        a * a * (M - 4) + 9 * (b * c) + (M - 5) * a * a * c) &
                       mask);
    as.push_back(a);
    bs.push_back(b);
    cs.push_back(c);
    o1s.push_back(o1);
  }

  solver.load_data(n,
                   {
                       {"a", as},
                       {"b", bs},
                       {"c", cs},
                   },
                   {{"o1", o1s}});
  auto result = solver.solve_output("o1");
  cout << solver.format_formula(result.value()) << '\n';
}
