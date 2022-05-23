#include <bits/stdc++.h>
using namespace std;

string trim(string s) {
    int l = 0;
    while (l < s.size()) {
        if (!isspace(s[l])) {
            break;
        }
        l++;
    }
    if (l == s.size()) {
        return "";
    }
    int r = s.size() - 1;
    while (r > l) {
        if (!isspace(s[r])) {
            break;
        }
        r--;
    }
    return s.substr(l, r + 1 - l);
}

struct Gate {
    string type;
    vector<Gate*> inputs;
    vector<Gate*> outputs;
};

struct Wire {
    Gate* input;
    vector<Gate*> outputs;
};

pair<int, string> process_word_desc(string line) {
    if (line[0] == '[') {
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == ']') {
                string word_desc = trim(line.substr(1, i - 1));
                line = trim(line.substr(i + 1));
                //cout << "word desc " << word_desc << '\n';
                size_t colon = word_desc.find(":");
                assert(colon != string::npos);
                int l = stoi(word_desc.substr(0, colon));
                int r = stoi(word_desc.substr(colon + 1));
                assert(r == 0);
                return {l + 1, line};
            }
        }
        assert(false);
    }
    return {-1, line};
}

void process_input(string line, vector<pair<int, string>> &inputs) {
    line = trim(line.substr(5));
    pair<int, string> result = process_word_desc(line);
    line = result.second;
    int width = result.first;

    line.push_back(',');
    string now;
    for (int i = 0; i < line.size(); i++) {
        if (line[i] == ',') {
            inputs.push_back({width, trim(now)});
            now = "";
            continue;
        }
        now.push_back(line[i]);
    }
}

void process_output(string line, vector<pair<int, string>> &outputs) {
    line = trim(line.substr(6));
    pair<int, string> result = process_word_desc(line);
    line = result.second;
    int width = result.first;

    line.push_back(',');
    string now;
    for (int i = 0; i < line.size(); i++) {
        if (line[i] == ',') {
            outputs.push_back({width, trim(now)});
            now = "";
            continue;
        }
        now.push_back(line[i]);
    }
}

void process_wire(string line, map<string, Wire> &wires) {
    line = trim(line.substr(4));
    pair<int, string> result = process_word_desc(line);
    line = result.second;
    int width = result.first;

    if (width == -1) {
        line.push_back(',');
        string now;
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == ',') {
                wires[trim(now)].input = nullptr;
                now = "";
                continue;
            }
            now.push_back(line[i]);
        }
    }
    else {
        line.push_back(',');
        string now;
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == ',') {
                now = trim(now);
                for (int j = 0; j < width; j++) {
                    wires[now + "[" + to_string(j) + "]"].input = nullptr;
                }
                now = "";
                continue;
            }
            now.push_back(line[i]);
        }
    }
}

string identify_gate(string &line) {
    vector<string> names = {"and", "or", "nand", "nor", "not", "buf", "xor", "xnor"};
    for (string name : names) {
        int n = name.size();
        if (line.substr(0, n) == name) {
            line = trim(line.substr(n));
            return name;
        }
    }
    //cout << line << '\n';
    assert(false);
}

void process_gate(string line, map<string, Wire> &wires) {
    string name = identify_gate(line);
    //cout << "here " << line << '\n';
    size_t para = line.find("(");
    assert(para != string::npos);
    line = line.substr(para);
    assert(line[0] == '(');
    assert(line[line.size() - 1] == ')');
    line = line.substr(1, line.size() - 2) + ",";
    //cout << "here2 " << line << '\n';
    string now;
    bool is_output = true;
    Gate *gate = new Gate();
    gate->type = name;
    for (int i = 0; i < line.size(); i++) {
        if (line[i] == ',') {
            now = trim(now);
            if (is_output) {
                wires[now].input = gate;
                is_output = false;
            }
            else {
                wires[now].outputs.push_back(gate);
            }
            now = "";
            continue;
        }
        now.push_back(line[i]);
    }
}

int main() {
    ifstream source("release/test01/top_primitive.v");
    
    vector<pair<int, string>> inputs;
    vector<pair<int, string>> outputs;

    map<string, Wire> wires;
    string line;
    while (getline(source, line, ';')) {
        line = trim(line);
        //cout << '<' << line << '>' << '\n';
        if (line.substr(0, 6) == "module") {
            continue;
        }
        if (line.substr(0, 9) == "endmodule") {
            continue;
        }
        if (line.substr(0, 5) == "input") {
            process_input(line, inputs);
            continue;
        }
        if (line.substr(0, 6) == "output") {
            process_output(line, outputs);
            continue;
        }
        if (line.substr(0, 4) == "wire") {
            process_wire(line, wires);
            continue;
        }
        process_gate(line, wires);
    }
    for (pair<int, string> in : inputs) {
        cout << in.first << ' ' << in.second << '\n';
        if (in.first == -1) {
            Gate *gate = new Gate();
            gate->type = "input";
            wires[in.second].input = gate;
        }
        else {
            Gate *gate = new Gate();
            gate->type = "input";
            for (int i = 0; i < in.first; i++) {
                wires[in.second + "[" + to_string(i) + "]"].input = gate;
            }
        }
    }
    for (pair<int, string> out : outputs) {
        cout << out.first << ' ' << out.second << '\n';
        if (out.first == -1) {
            Gate *gate = new Gate();
            gate->type = "output";
            wires[out.second].outputs.push_back(gate);
        }
        else {
            Gate *gate = new Gate();
            gate->type = "output";
            for (int i = 0; i < out.first; i++) {
                wires[out.second + "[" + to_string(i) + "]"].outputs.push_back(gate);
            }
        }
    }
    cout << "all wires\n";
    for (auto p : wires) {
        cout << p.first << '\n';
        Wire wire = p.second;
        Gate *input = wire.input;
        for (Gate *output : wire.outputs) {
            input->outputs.push_back(output);
            output->inputs.push_back(output);
        }
    }
}
