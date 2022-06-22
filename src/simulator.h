#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include <bits/stdc++.h>
#include "parser.h"

using namespace std;

class Simulator: public Parser {

public:
    Simulator() {};
    ~Simulator() {};
    
    void generate_output(map<string, vector<bitvec>>&);
    void generate_input(int num=1000);
    map<string, vector<bitvec>> input_testcase;
    map<string, vector<bitvec>> output_testcase;
private:
    bitvec calculate(GateType gateType, bitvec v1, bitvec v2={});
    void process_wire(string line);
    void print_val(string name, bitvec);
};


#endif