#include "parser.h"
#include "simulator.h"
#include "linear.h"
#include "converter.h"
#include <bits/stdc++.h>
#define METHOD_1 1
#define METHOD_2 2
#define CUR_METHOD 1

void StructurallyEquivalentReduction(const string& input, const string& output) {
    Parser parser;
    parser.read(input);
    parser.strash();
    parser.write(output);
}

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

    Simulator simulator;
    simulator.read(input);
    simulator.generate_input();
    simulator.generate_output(simulator.input_testcase);
    cout << "start solving..." << endl;
    auto result = solve(simulator.input_testcase, simulator.output_testcase);
    result.first ? cout << "solved" << endl : cout << "can't solved" << endl;
    if (result.first) {
        convert(output, result.second, simulator.input_info, simulator.output_info);  
        cout << "solved by Polynomial Coefficient Recovering" << endl;
    }
    else {
        StructurallyEquivalentReduction(input, output);
        cout << "solved by Structurally Equivalent Reduction" << endl;
    }

    /*
    if (CUR_METHOD == METHOD_2) {
        Parser parser;
        parser.read(input);
        parser.strash();
        parser.write(output);
    
    // vector<Out*> outputs = parser.getOut();
    // for (int i = 0; i < outputs.size(); ++i) {
    //     for (int j = 0; j < outputs[i]->outs.size(); ++j) {
    //         //cout << "name: " << outputs[i]->outs[j]->getName() << endl;
    //         parser.dfsFanin(outputs[i]->outs[j]);
    //     }
    //     cout << endl;
    // }
    
    }

    else if (CUR_METHOD == METHOD_1) {
        Simulator simulator;
        //argc < 2 ? simulator.read("release/test01/top_primitive.v") : simulator.read(argv[1]);
        simulator.read(input);
        simulator.generate_input();
        simulator.generate_output(simulator.input_testcase);
        cout << "start solving..." << endl;
        auto result = solve(simulator.input_testcase, simulator.output_testcase);
        result.first ? cout << "solved" << endl : cout << "can't solved" << endl;
        // string outPath = "out.v";
        if (result.first) convert(output, result.second, simulator.input_info, simulator.output_info);
    }
    */
    return 0;
}
