#include "parser.h"
#include "simulator.h"
#include "linear.h"
#include "converter.h"
#include <bits/stdc++.h>
#define METHOD_1 1
#define METHOD_2 2
#define CUR_METHOD 1
int main(int argc, char **argv) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " -input <inputFile> -output <outputFile>" << endl;
        return 0;
    }
    string arg1 = argv[1];
    string arg2 = argv[2];
    string arg3 = argv[3];
    string arg4 = argv[4];
    string input, output;
    if (arg1 == "-input") {
        if (arg3 != "-output") {
            cerr << "Usage: " << argv[0] << "-input <inputFile> -output <outputFile>" << endl;
            return 0;
        }
        input = arg2;
        output = arg4;
    }
    else {
        if (arg1 != "-output" || arg3 != "-input") {
            cerr << "Usage: " << argv[0] << "-input <inputFile> -output <outputFile>" << endl;
            return 0;
        }
        input = arg4;
        output = arg2;
    }

    if (CUR_METHOD == METHOD_2) {
    Parser parser;
    parser.read(input);
    parser.strash();
    parser.write(output);
    /*
    vector<Out*> outputs = parser.getOut();
    for (int i = 0; i < outputs.size(); ++i) {
        for (int j = 0; j < outputs[i]->outs.size(); ++j) {
            //cout << "name: " << outputs[i]->outs[j]->getName() << endl;
            parser.dfsFanin(outputs[i]->outs[j]);
        }
        cout << endl;
    }
    */
    }

    else if (CUR_METHOD == METHOD_1) {
        Simulator simulator;
        //argc < 2 ? simulator.read("release/test01/top_primitive.v") : simulator.read(argv[1]);
        simulator.read(input);
        // cout << "generating inputs..." << endl;
        simulator.generate_input();
        // cout << "input generated\nstart generating outputs..." << endl;
        simulator.generate_output(simulator.input_testcase);
        // cout << "output generated\nstart solving..." << endl;
        auto result = solve(simulator.input_testcase, simulator.output_testcase);
        result.first ? cout << "solved" << endl : cout << "can't solved, using condition to solve..." << endl;
        // string outPath = "out.v";
        Converter converter(simulator.input_info, simulator.output_info);
        if (result.first) converter.convert(output, result.second);
        else {
            pair<string, int> possibly_control = simulator.find_smallest_input();
            int testVal = 0;
            simulator.generate_input(possibly_control.first, testVal);
            simulator.generate_output(simulator.input_testcase);
            auto conditional_result = solve(simulator.input_testcase, simulator.output_testcase);
            if (conditional_result.first) {
                int sz = possibly_control.second == -1 ? 0 : (int) pow(2, possibly_control.second);
                for (int i = 0; i < sz; i++) {
                    // cout << i << endl;
                    simulator.generate_input(possibly_control.first, i);
                    simulator.generate_output(simulator.input_testcase);
                    auto sep_result = solve(simulator.input_testcase, simulator.output_testcase);
                    if (!sep_result.first) {
                        cout << "can't solve using conditions\n";
                        break;
                    }
                    else {
                        converter.convert_control(output, sep_result.second, possibly_control.first, i);
                    }
                }
                converter.convert_control(output);
                cout << "solved using condition" << endl;
            }
            else cout << "still can't solve using conditions" << endl;
        }
    }

    return 0;
}
