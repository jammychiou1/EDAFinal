#include "converter.h"

using namespace std;
#define INDENT_UNIT string("  ")

void 
Converter::convert(string path, vector<map<vector<int>, __int128>> result) {
    ofstream file(path);
    // for (auto mp: result) {
    //     for (auto term: mp) {
    //         for (int a: term.first) {
    //             cout << a << " ";
    //         }
    //         cout << endl << (int)term.second << endl;
    //     }
    // }
    Converter::prewrite(file);
    Converter::inwrite(file, result);
    Converter::postwrite(file);
}   

void 
Converter::convert_control(string path, vector<map<vector<int>, __int128>> result, string control_input, int val) {
    ofstream file;
    if (!pre) {
        pre = true;
        file.open(path);
        Converter::prewrite(file);
    }
    else {
        file.open(path, ios_base::app);
    }
    if (result.size() != 0) {
        inwrite(file, result, control_input, val);
    }
    else {
        Converter::postwrite(file);
    }
    
};


string
Converter::generate_lines(std::vector<std::pair<std::string, int>>& group, std::string type) {
    string paragraph = "";
    for (auto& item: group) {
        string line = INDENT_UNIT + type + " ";
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

void
Converter::prewrite(ofstream& file) {
    string module_start = "module top(";
    for (auto& in: inputs) module_start += in.first + ", ";
    for (auto& out: outputs) module_start += out.first + ", ";
    module_start = module_start.substr(0, module_start.size() - 2) + ");\n";
    file << module_start;

    file << Converter::generate_lines(inputs, "input");
    file << Converter::generate_lines(outputs, "output");
    file << Converter::generate_lines(inputs, "wire");
    file << Converter::generate_lines(outputs, "wire");
}

void
Converter::inwrite(ofstream& file, vector<map<vector<int>, __int128>> result, string control_input, int val) {
    string indent = INDENT_UNIT;
    if (control_input != "") {
        string line = indent;
        if (!start_cond) {
            start_cond = true;
            line += "if (";
        }
        else line += "else if (";
        line += control_input + " == " + to_string(val) + ") begin\n";
        file << line;
        indent += indent;
    }
    for (size_t i = 0; i < outputs.size(); i++) {
        string line = indent + "assign " + outputs[i].first + " = ";
        bool onlyZero = true;
        bool isConstant = true;
        for(auto& term: result[i]) {
            int coeff = (int)term.second;
            if (isConstant) {
                isConstant = false;
                if (coeff != 0) {
                    onlyZero = false;
                    line += to_string(coeff) + " + ";
                } 
            }
            else if (coeff != 0) {
                onlyZero = false;
                if (coeff < 0) {
                    line[line.size() - 2] = '-';
                    coeff = -coeff;
                }
                if (coeff != 1) {
                    line += to_string(coeff) + "*";
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
        if (onlyZero) {
            line += "0;\n";
        }
        else {
            line = line.substr(0, line.size() - 3) + ";\n";
        }
        file << line;
    }
    if (control_input != "") {
        file << INDENT_UNIT << "end\n";
    }
}

void 
Converter::postwrite(ofstream& file) {
    string module_end = "endmodule\n";
    file << module_end;
    file.close();
}
