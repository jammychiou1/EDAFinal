#include <bits/stdc++.h>
#include "linear.h"
using namespace std;
typedef unsigned __int128 u128;
typedef vector<u128> vec;
typedef vector<vec> mat;

vector<bool> from_int(int a, int width) {
    vector<bool> ans(width);
    for (int i = 0; i < width; i++) {
        if (a & (1ll << i)) {
            ans[width - 1 - i] = true;
        }
    }
    return ans;
}

int main() {
    //Mat A(15);
    //A.data = mat{{4, 2}, {0, 4}, {1, 1}};
    //auto [D, L, R] = smith_form(A);
    //D.print();
    //L.print();
    //R.print();
    //(L * A * R).print();

    //int test = inv(3, 16);
    //cout << test << '\n';

    //mat B{{4, 2}, {0, 4}, {1, 1}};
    //vec b{10, 12, 10};
    //solve_mod_pow_2(B, b, 4);

    map<string, vector<vector<bool>>> inputs;
    map<string, vector<vector<bool>>> outputs;
    for (int i = 0; i < 1000; i++) {
        u128 a = rand() % 16;
        u128 b = rand() % 16;
        u128 c = rand() % 16;
        u128 mask = 1023;
        inputs["in1"].push_back(from_int(a, 4));
        inputs["in2"].push_back(from_int(b, 4));
        inputs["in3"].push_back(from_int(c, 4));
        //inputs["in4"].push_back(from_int(rand(), 4));
        //inputs["in5"].push_back(from_int(rand(), 4));
        //inputs["in6"].push_back(from_int(rand(), 4));
        outputs["out1"].push_back(from_int((100 + 200 * a * b - 3 * b * c) & mask, 10));
        outputs["out2"].push_back(from_int((a * a + b * b) & mask, 10));
    }
    auto result = solve(inputs, outputs);
    cout << result.first << '\n';
    for (auto mp : result.second) {
        for (auto term : mp) {
            print_termdec(term.first);
            cout << (int) term.second << '\n';
        }
    }
}
