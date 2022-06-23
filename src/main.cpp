#include "parser.h"
#include "simulator.h"
#include "linear.h"
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
        for (auto mp : result.second) {
            for (auto term : mp) {
                print_termdec(term.first);
                cout << (int) term.second << '\n';
            }
            cout << '\n';
            // for (int i = 0; i < 5; i++) {
            //     cout << i << endl;
            //     for (auto it = simulator.input_testcase.begin(); it != simulator.input_testcase.end(); it++) {
            //         cout << it->first << endl;
            //         for (int k = 0; k < it->second[0].size(); k++) 
            //             cout << it->second[i][k];
            //         cout << endl;
            //     }
            //     for (auto it = simulator.output_testcase.begin(); it != simulator.output_testcase.end(); it++) {
            //         cout << it->first << endl;
            //         for (int k = 0; k < it->second[0].size(); k++) 
            //             cout << it->second[i][k];
            //         cout << endl;
            //     }
            //     cout << endl;
            // }
        }
    }
    
    


    return 0;
}
