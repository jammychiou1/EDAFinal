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

  int n = 300;
  Mat A(n, 20), y(n, 1);
  BigInt M(512);
  BigInt mask(M - 1);
  for (int i = 0; i < n; i++) {
    BigInt a = BigInt::randm(state, M);
    BigInt b = BigInt::randm(state, M);
    BigInt c = BigInt::randm(state, M);
    BigInt o1 = BigInt((2 * a + 4 * b + (M - 1) * c + 3 * a * b + a * a * (M - 4) + 9 * (b * c) + (M - 5) * a * a * c + (M - 3) * (BigInt(1) << c.to<ulong>())) & mask);
    A.at(i, 0) = 1;
    A.at(i, 1) = a;
    A.at(i, 2) = b;
    A.at(i, 3) = c;
    A.at(i, 4) = (a * b) & mask;
    A.at(i, 5) = (a * c) & mask;
    A.at(i, 6) = (b * c) & mask;
    A.at(i, 7) = (a * a) & mask;
    A.at(i, 8) = (b * b) & mask;
    A.at(i, 9) = (c * c) & mask;
    A.at(i, 10) = (a * a * b) & mask;
    A.at(i, 11) = (a * a * c) & mask;
    A.at(i, 12) = (b * b * a) & mask;
    A.at(i, 13) = (b * b * c) & mask;
    A.at(i, 14) = (c * c * a) & mask;
    A.at(i, 15) = (c * c * b) & mask;
    A.at(i, 16) = (a * b * c) & mask;
    A.at(i, 17) = BigInt(1) << a.to<ulong>();
    A.at(i, 18) = BigInt(1) << b.to<ulong>();
    A.at(i, 19) = BigInt(1) << c.to<ulong>();
    y.at(i, 0) = o1;
  }

  // A.print_pretty();
  // cout << '\n';
  // y.print_pretty();
  // cout << '\n';
  // cout << '\n';

  auto s = short_solution(A, y, BigInt(mask));
  s->print_pretty();
  cout << '\n';

  array ans = {0, 2, 4, -1, 3, 0, 9, -4, 0, 0, 0, -5, 0, 0, 0, 0, 0, 0, 0, -3};
  for (int i = 0; i < ans.size(); i++) {
    assert(s->at(i, 0) == ans[i]);
  }
}
