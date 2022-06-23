#include "converter.h"

using namespace std;
#define INDENT string("  ")

void 
convert(string path, vector<map<vector<int>, __int128>> result, vector<pair<string, int>>& inputs, vector<pair<string, int>>& outputs) {
    ofstream file(path);
    // for (auto mp: result) {
    //     for (auto term: mp) {
    //         for (int a: term.first) {
    //             cout << a << " ";
    //         }
    //         cout << endl << (int)term.second << endl;
    //     }
    // }
    string module_start = "module top(";
    for (auto& in: inputs) module_start += in.first + ", ";
    for (auto& out: outputs) module_start += out.first + ", ";
    module_start = module_start.substr(0, module_start.size() - 2) + ");\n";
    file << module_start;

    file << generate_lines(inputs, "input");
    file << generate_lines(outputs, "output");
    file << generate_lines(inputs, "wire");
    file << generate_lines(outputs, "wire");
    for (size_t i = 0; i < outputs.size(); i++) {
        string line = INDENT + "assign " + outputs[i].first + " = ";
        bool isConstant = true;
        for(auto& term: result[i]) {
            int coeff = (int)term.second;
            if (isConstant) {
                isConstant = false;
                if (coeff != 0) {
                    line += to_string(coeff) + " + ";
                } 
            }
            else if (coeff != 0) {
                if (coeff < 0) {
                    line[line.size() - 2] = '-';
                    coeff = -coeff;
                }
                if (coeff != 1) {
                    line += to_string((int)term.second) + "*";
                }
                for (size_t j = 0; j < term.first.size(); j++) {
                    int num = term.first[j];
                    while (num --) {
                        line += inputs[j].first + "*";
                    }
                }
                line.pop_back();
                line += " + ";
            }
        }
        line = line.substr(0, line.size() - 3) + ";\n";
        file << line;
    }

    string module_end = "endmodule\n";
    file << module_end;
    file.close();
}

string
generate_lines(std::vector<std::pair<std::string, int>>& group, std::string type) {
    string paragraph = "";
    for (auto& item: group) {
        string line = INDENT + type + " ";
        if (item.second == -1) {
            line += item.first + ";\n";
        }
        else {
            line = line + "[" + to_string(item.second - 1) + ":0] " + item.first + ";\n";
        }
        paragraph += line;
    }
    return paragraph;
}
