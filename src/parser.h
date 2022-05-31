#ifndef __PARSER_H__
#define __PARSER_H__

#include <bits/stdc++.h>

using namespace std;

enum Type {
    GATE,
    NET,
    INPUT,
    OUTPUT
};

enum GateType {
    AND,
    OR,
    NAND,
    NOR,
    NOT,
    BUF,
    XOR,
    XNOR
};

class Based {

friend class Parser;
friend struct In;
friend struct Out;
//friend struct Net;

public:
    Based(const string& name, Type type, int id = 0) : name(name), id(id), type(type) {};
    virtual ~Based() {};
    void addFanin(Based*);
    void addFanout(Based*);
    string getName() const { return name;}

protected:
    string name;
    int id;
    Type type;
    GateType gateType;
    vector<Based*> fanins;
    vector<Based*> fanouts;
};


struct In {
    string name;
    int id;
    int width; // width = ins.size() or -1 for scalar
    vector<Based*> ins;
    In(const string& _name, int _width, int _id = 0) {
        name = _name;
        id = _id;
        width = _width;
        if (width == -1)
            ins.push_back(new Based(name, INPUT, id));
        else {
            ins.reserve(width);
            for (int i = 0; i < width; ++i) {
                ins.push_back(new Based(name + "[" + to_string(i) + "]", INPUT, id));
            }
        }
    }
};
struct Out {
    string name;
    int id;
    int width; // width = outs.size() or -1 for scalar
    vector<Based*> outs;
    Out(const string& _name, int _width, int _id = 0) {
        name = _name;
        id = _id;
        width = _width;
        if (width == -1)
            outs.push_back(new Based(name, OUTPUT, id));
        else {
            outs.reserve(width);
            for (int i = 0; i < width; ++i) {
                outs.push_back(new Based(name + "[" + to_string(i) + "]", OUTPUT, id));
            }
        }
    }
};
/*
struct Net {
    string name;
    int id;
    int width; // width = wires.size()
    vector<Based*> wires;
    Net(const string& _name, int _width, int _id = 0) {
        name = _name;
        id = _id;
        width = _width;
        wires.reserve(width);
        for (int i = 0; i < width; ++i) {
            wires.push_back(new Based(name, NET, id));
        }
    }
};
*/


class Parser {
public:
    Parser(){}
    ~Parser() {
        // remember to delete pointer
    }
    void read(const string&);
    vector<In*> getIns() const { return inputs; }
    vector<Out*> getOut() const { return outputs; }
    void dfsFanin(Based* ptr = nullptr) const;

private:
    void process_input(string line);
    void process_output(string line);
    void process_wire(string line);
    void process_gate(string line);
    void dfsFanin_helper(Based*, int) const;

    static string trim(const string&);
    static pair<int, string> process_word_desc(string line);
    static GateType identify_gate(string &line);

    vector<In*> inputs;
    vector<Out*> outputs;
    vector<Based*> gates;
    map<string, Based*> inputsMap;   // to quick find pointer(Based*) from name(string)
    map<string, Based*> outputsMap; // to quick find pointer(Based*) from name(string)
    map<string, Based*> wires;
    //vector<Based*> nets;
};

#endif  // __PARSER_H__