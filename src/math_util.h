#ifndef __MATH_UTIL_H__
#define __MATH_UTIL_H__

#include <flint/fmpz_matxx.h>
#include <flint/fmpzxx.h>

#include <array>
#include <optional>

typedef flint::fmpz_matxx Mat;
typedef flint::fmpzxx BigInt;

void add_c_row_i_to_j(Mat &A, int i, int j, BigInt c, BigInt mask,
                      bool use_mask = true);

void add_c_col_i_to_j(Mat &A, int i, int j, BigInt c, BigInt mask,
                      bool use_mask = true);

void swap_row_i_j(Mat &A, int i, int j);

void swap_col_i_j(Mat &A, int i, int j);

bool elim_col(Mat &A, Mat &L, int now, BigInt mask);

bool elim_row(Mat &A, Mat &R, int now, BigInt mask);

bool make_pivot(Mat &A, Mat &L, Mat &R, int now);

// return {D, L, R}
// D = L A R
std::array<Mat, 3> smith_normal_form(Mat A, BigInt mask);

void lll_reduce(Mat &L);

// Minkowski's first theorem:
// For Lattice L of dimension n, shortest nonzero vector in L has norm less than
// (about) sqrt(n) det(L)^(1/n).
//
// Property of LLL-reduced lattice:
// First basis vector returned in LLL is not much longer than shortest nonzero
// vector.
Mat closest_vector_embedded(Mat L, Mat y, BigInt mask);

BigInt div_round(BigInt p, BigInt q);

Mat closest_vector_babai(Mat L, Mat y, BigInt mask);

std::optional<Mat> short_solution(Mat A, Mat b, BigInt mask);

BigInt to_mask(int bit_width);

#endif  // __MATH_UTIL_H__
