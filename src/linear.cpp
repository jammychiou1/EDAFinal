#include <cassert>

#include "linear.h"

using namespace std;

typedef unsigned __int128 u128;
typedef __int128 i128;
typedef vector<vector<u128>> mat;
typedef vector<u128> vec;

string u128_to_string(u128 val) {
    if (val == 0) return "0";
    string s;
    while (val != 0) {
        s = char('0' + val % 10) + s;
        val /= 10;
    }
    return s;
}

void Mat::swap_row_or_col(int i, int j, bool transpose) {
    for (int k = 0; k < get_h_or_w(!transpose); k++) {
        swap(at(i, k, transpose), at(j, k, transpose));
    }
}

void Mat::add_row_or_col_i_to_j(int i, int j, u128 c, bool transpose) {
    for (int k = 0; k < get_h_or_w(!transpose); k++) {
        at(j, k, transpose) = (at(j, k, transpose) + c * at(i, k, transpose)) & mask;
    }
}

Mat::Mat(int n, u128 mask) : Mat(pair<int, int>{n, n}, mask){
    for (int i = 0; i < n; i++) {
        data[i][i] = 1;
    }
}

Mat::Mat(pair<int, int> shape, u128 mask) {
    data = mat(shape.first, vec(shape.second, 0));
    this->mask = mask;
}

Mat::Mat(vector<unsigned __int128> v, u128 mask) : Mat(pair<int, int>{v.size(), 1}, mask) {
    for (int i = 0; i < v.size(); i++) {
        data[i][0] = v[i];
    }
}

Mat Mat::operator*(const Mat& A) {
    assert(mask == A.mask);
    assert(get_h_or_w(true) == A.get_h_or_w(false));
    Mat result(pair<int, int>{get_h_or_w(false), A.get_h_or_w(true)}, mask);
    for (int i = 0; i < get_h_or_w(false); i++) {
        for (int j = 0; j < A.get_h_or_w(true); j++) {
            for (int k = 0; k < get_h_or_w(true); k++) {
                result.data[i][j] = (result.data[i][j] + data[i][k] * A.data[k][j]) & mask;
            }
        }
    }
    return result;
}

bool make_pivot(Mat& A, Mat& L, Mat& R, int start) {
    for (int i = start; i < A.get_h_or_w(false); i++) {
        for (int j = start; j < A.get_h_or_w(true); j++) {
            if (A.at(i, j, false) != 0) {
                A.swap_row_or_col(start, i, false);
                A.swap_row_or_col(start, j, true);
                L.swap_row_or_col(start, i, false);
                R.swap_row_or_col(start, j, true);
                return true;
            }
        }
    }
    return false;
}

bool improve_pivot(Mat& A, Mat& L_or_R, int i, bool transpose) {
    bool flag = true;
    for (int i2 = i + 1; i2 < A.get_h_or_w(transpose); i2++) {
        //cout << "i2 is now " << i2 << '\n';
        if (A.at(i2, i, transpose) != 0) {
            flag = false;
            while (A.at(i, i, transpose) != 0) {
                //cout << "i, i and i2, i " << u128_to_string(A.at(i, i, transpose)) << ' ' << u128_to_string(A.at(i2, i, transpose)) << '\n';
                i128 c = A.at(i2, i, transpose) / A.at(i, i, transpose);
                //cout << "c " << u128_to_string(c) << '\n';
                //cout << "-c " << u128_to_string((-c) & A.mask) << '\n';
                //cout << "A\n";
                //A.print();
                //cout << endl;
                //char d;
                //cin >> d;
                A.add_row_or_col_i_to_j(i, i2, ((~c) + 1) & A.mask, transpose);
                A.swap_row_or_col(i, i2, transpose);
                L_or_R.add_row_or_col_i_to_j(i, i2, ((~c) + 1) & A.mask, transpose);
                L_or_R.swap_row_or_col(i, i2, transpose);
            }
            A.swap_row_or_col(i, i2, transpose);
            L_or_R.swap_row_or_col(i, i2, transpose);
        }
        //cout << (unsigned long long) A.at(i, i, transpose) << ' ' << (unsigned long long) A.at(i2, i, transpose) << '\n';
        //cout << "A\n";
        //A.print();
        //cout << endl;
        //char d;
        //cin >> d;
    }
    return flag;
}

tuple<Mat, Mat, Mat> smith_form(Mat A) {
    Mat L(A.get_h_or_w(false), A.mask), R(A.get_h_or_w(true), A.mask);

    for (int i = 0; i < min(A.get_h_or_w(false), A.get_h_or_w(true)); i++) {
        //cout << "i " << i << '\n';
        if (!make_pivot(A, L, R, i)) {
            break;
        }
        while (true) {
            bool flag = true;
            flag &= improve_pivot(A, L, i, false);
            flag &= improve_pivot(A, R, i, true);
            if (flag) {
                break;
            }
        }
    }

    return {A, L, R};
}

u128 lowbit(u128 a) {
    return a & (-a);
}

pair<i128, i128> extgcd(i128 a, i128 b) {
    if (b == 0) {
        return {1, 0};
    }
    i128 k = a / b, c = a % b;
    auto [n, m] = extgcd(b, c);
    return {m, n - m * k};
}

u128 inv(u128 a, u128 mod) {
    i128 tmp = extgcd(a, mod).first;
    if (tmp < 0) {
        tmp = (mod - ((-tmp) % mod));
    }
    return tmp % mod;
}

vec sample(const vector<vec>& yss) {
    vec res;
    for (int i = 0; i < yss.size(); i++) {
        res.push_back(yss[i][rand() % yss[i].size()]);
    }
    return res;
}

pair<bool, vec> solve_mod_pow_2(mat AA, vec bb, u128 mod) {
    Mat A(mod - 1);
    A.data = AA;
    Mat b(bb, mod - 1);

    int n = A.get_h_or_w(false), m = A.get_h_or_w(true);

    assert(b.get_h_or_w(true) == 1);
    assert(n > m);

    auto [D, L, R] = smith_form(A);

    //cout << "A\n";
    //A.print();
    //cout << "b\n";
    //b.print();
    //cout << "D\n";
    //D.print();
    //cout << "L\n";
    //L.print();
    //cout << "R\n";
    //R.print();

    Mat Lb = L * b;

    //cout << "Lb\n";
    //Lb.print();

    vector<vec> yss(m);
    for (int i = 0; i < A.get_h_or_w(false); i++) {
        if (i >= D.get_h_or_w(true) || D.data[i][i] == 0) {
            if (Lb.data[i][0] != 0) {
                return {false, vec()};
            }
            if (i < D.get_h_or_w(true)) {
                yss[i].push_back(0);
            }
            continue;
        }
        u128 di = D.data[i][i];
        u128 Lbi = Lb.data[i][0];

        if (Lbi == 0) {
            yss[i].push_back(0);
            continue;
        }
        if (lowbit(di) > lowbit(Lbi)) {
            return {false, vec()};
        }
        u128 gcd = lowbit(di);
        u128 tmp = di / gcd;
        u128 base = Lbi / gcd * inv(tmp, mod) % mod;
        //cout << "base " << u128_to_string(base) << '\n';
        for (u128 y = base; y < mod; y += mod / gcd) {
            yss[i].push_back(y);
            //cout << u128_to_string(y) << ' ';
        }
        //cout << '\n';
    }
    int mn = 1e9;
    vec bst_y;
    for (int i = 0; i < 10000; i++) {
        vec y = sample(yss);
        int score = 0;
        for (u128 a : y) {
            if (a != 0) {
                score++;
            }
        }
        if (score < mn) {
            mn = score;
            bst_y = y;
        }
    }
    //cout << "here\n" << '\n';
    return {true, (R * Mat(bst_y, mod - 1)).to_vec()};
}

u128 to_u128(vector<bool> a) {
    u128 res = 0;
    for (bool b : a) {
        res <<= 1;
        res += b;
    }
    return res;
}

u128 calc_term(const vector<u128>& input_sample, const termdec& term, u128 mask) {
    u128 res = 1;
    for (int i = 0; i < input_sample.size(); i++) {
        for (int j = 0; j < term[i]; j++) {
            res = (res * input_sample[i]) & mask;
        }
    }
    return res;
}

vector<u128> gen_row(const vector<u128>& input_sample, const vector<termdec>& terms, u128 mask) {
    vector<u128> res;
    for (int i = 0; i < terms.size(); i++) {
        res.push_back(calc_term(input_sample, terms[i], mask));
    }
    return res;
}

void n_m_simple_terms(int n, int m, int num_inputs, vector<termdec>& result, termdec& term_now, int prefix_sum = 0) {
    if (term_now.size() >= num_inputs) {
        result.push_back(term_now);
        return;
    }
    for (int i = 0; i <= n; i++) {
        if (prefix_sum + i <= m) {
            term_now.push_back(i);
            n_m_simple_terms(n, m, num_inputs, result, term_now, prefix_sum + i);
            term_now.pop_back();
        }
    }
}

void print_termdec(termdec term) {
    cout << '(';
    for (int a : term) {
        cout << a << ' ';
    }
    cout << ")\n";
}

pair<bool, map<termdec, i128>> solve_single(map<string, vector<u128>> inputs, vector<u128> output, int width, int n, int m) {
    int count = output.size();
    u128 mod = u128(1) << width;
    u128 mask = mod - 1;
    mat AA;

    return {false, map<termdec, i128>()};
}

pair<bool, vector<map<termdec, i128>>> solve(map<string, vector<bitvec>> inputs, map<string, vector<bitvec>> outputs) {
    if (inputs.size() > 6) {
        // cout << "here\n";
        return {false, vector<map<termdec, i128>>()};
    }
    for (auto &p : inputs) {
        if (p.second[0].size() >= 100) {
            return {false, vector<map<termdec, i128>>()};
        }
    }
    for (auto &p : outputs) {
        if (p.second[0].size() >= 100) {
            return {false, vector<map<termdec, i128>>()};
        }
    }

    map<string, vector<u128>> inputs_u128;
    for (auto &p : inputs) {
        for (bitvec v : p.second) {
            inputs_u128[p.first].push_back(to_u128(v));
        }
    }

    vector<map<termdec, i128>> ans;
    for (auto &p : outputs) {
        int width = p.second[0].size();
        vector<u128> output_u128;
        for (bitvec v : p.second) {
            output_u128.push_back(to_u128(v));
        }

        pair<bool, map<termdec, i128>> res = solve_single(inputs_u128, output_u128, width, 1, 5);
        if (res.first) {
            ans.push_back(res.second);
            continue;
        }

        res = solve_single(inputs_u128, output_u128, width, 2, 5);
        if (res.first) {
            ans.push_back(res.second);
            continue;
        }

        res = solve_single(inputs_u128, output_u128, width, 3, 5);
        if (res.first) {
            ans.push_back(res.second);
            continue;
        }
        return {false, vector<map<termdec, i128>>()};
    }
    return {true, ans};
}
