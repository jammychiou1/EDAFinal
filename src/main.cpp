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

    if (CUR_METHOD == METHOD_2) {
    Parser parser;
    if (argc < 2) 
        parser.read("release/test01/top_primitive.v");
    else
        parser.read(argv[1]);
    parser.strash();
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
        argc < 2 ? simulator.read("release/test01/top_primitive.v") : simulator.read(argv[1]);
        simulator.generate_input();
        simulator.generate_output(simulator.input_testcase);
        auto result = solve(simulator.input_testcase, simulator.output_testcase);
        cout << result.first << endl;
        string outPath = "out.v";
        if (result.first) convert(outPath, result.second, simulator.input_info, simulator.output_info);
    }

    return 0;
}
