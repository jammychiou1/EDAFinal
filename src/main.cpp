#include "parser.h"
#include <bits/stdc++.h>

int main(int argc, char **argv) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    Parser parser;
    if (argc < 2) 
        parser.read("release/test01/top_primitive.v");
    else
        parser.read(argv[1]);
    vector<Out*> outputs = parser.getOut();
    for (int i = 0; i < outputs.size(); ++i) {
        for (int j = 0; j < outputs[i]->outs.size(); ++j) {
            //cout << "name: " << outputs[i]->outs[j]->getName() << endl;
            parser.dfsFanin(outputs[i]->outs[j]);
        }
        cout << endl;
    }
    return 0;
}