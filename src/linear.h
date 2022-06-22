#include <utility>
#include <vector>
#include <map>
#include <iostream>
#include <tuple>

std::string u128_to_string(unsigned __int128 val);

struct Mat {
    public:
        unsigned __int128 mask;
        std::vector<std::vector<unsigned __int128>> data;
        // tranpose = 0/1: get h/w
        int get_h_or_w(bool transpose) const {return transpose ? data[0].size() : data.size();}
        const unsigned __int128& at(int i, int j, bool transpose) const {
            if (transpose) {
                return data[j][i];
            }
            else {
                return data[i][j];
            }
        }
        unsigned __int128& at(int i, int j, bool transpose) {
            if (transpose) {
                return data[j][i];
            }
            else {
                return data[i][j];
            }
        }

        void swap_row_or_col(int i, int j, bool transpose);

        void add_row_or_col_i_to_j(int i, int j, unsigned __int128 c, bool transpose);

        void print(bool transpose = false) const {
            //std::cout << get_h_or_w(transpose) << '\n';
            for (int i = 0; i < get_h_or_w(transpose); i++) {
                for (int j = 0; j < get_h_or_w(!transpose); j++) {
                    std::cout << u128_to_string(at(i, j, transpose)) << ' ';
                }
                std::cout << '\n';
            }
        }

        std::vector<unsigned __int128> to_vec() {
            //assert(get_h_or_w(true) == 1);
            std::vector<unsigned __int128> res;
            for (int i = 0; i < get_h_or_w(false); i++) {
                res.push_back(data[i][0]);
            }
            return res;
        }

        Mat operator*(const Mat& A);
        
        Mat(int n, unsigned __int128 mask);
        Mat(std::pair<int, int> shape, unsigned __int128 mask);
        Mat(std::vector<unsigned __int128>, unsigned __int128 mask);
        Mat(unsigned __int128 mask = 0) {this->mask = mask;}
};

//struct Vec {
//    std::vector<unsigned __int128> data;
//    int get_h() {return data.size();};
//};

//std::pair<bool, Vec> solve_mod_pow_2(Mat A, Vec b);

//void improve_pivot(Mat& A, Mat& L_or_R, int i, bool transpose);
std::tuple<Mat, Mat, Mat> smith_form(Mat A);
//std::pair<__int128, __int128> extgcd(__int128 a, __int128 b);
unsigned __int128 inv(unsigned __int128 a, unsigned __int128 mod);
std::pair<bool, Mat> solve_mod_pow_2(std::vector<std::vector<unsigned __int128>> AA, std::vector<unsigned __int128> bb, int pow);
std::pair<bool, std::map<std::vector<int>, __int128>> solve_single(std::map<std::string, std::vector<unsigned __int128>> inputs, std::vector<unsigned __int128> output, int width, int n, int m);

typedef std::vector<int> termdec;
typedef std::vector<bool> bitvec;

void print_termdec(termdec term);

// inputs: {
//     "in1": [[001], [010], [011]], // 1, 2, 3
//     "in2": [[011], [100], [101]]  // 3, 4, 5
// }
// outputs: {
//     "out1": [4, 6, 8] // in1 + in2
//     "out2": [3, 8, 15] // in1 * in2
// }
//
// return (success, solution)
// solution: [
//     {
//         (0, 0): 0, 
//         (0, 1): 1,
//         (1, 0): 1,
//         (1, 1): 0
//     }, // for out1
//     {
//         (0, 0): 0, 
//         (0, 1): 0,
//         (1, 0): 0,
//         (1, 1): 1
//     }, // for out2
// ]
std::pair<bool, std::vector<std::map<std::vector<int>, __int128>>> solve(std::map<std::string, std::vector<std::vector<bool>>> inputs, std::map<std::string, std::vector<std::vector<bool>>> outputs);
