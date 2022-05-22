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
    int r = s.size() - 1;
    while (r > l) {
        if (!isspace(s[r])) {
            break;
        }
        l++;
    }
    return s.substr(l, r + 1);
}

struct gate {
    string type;
    vector<gate*> inputs;
    vector<gate*> outputs;
};

struct wire {
    gate* input;
    vector<gate*> outputs;
};

pair<int, string> process_word_desc(string line) {
    if (line[0] == '[') {
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == ']') {
                line = trim(line.substr(i + 1));
                string word_desc = trim(line.substr(1, i));
                size_t colon = word_desc.find(":");
                assert(colon != npos);
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

void process_input(string line, vector<string> &inputs) {
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

void process_output(string line, vector<string> &outputs) {
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

void process_wire(string line, map<string, wire> &wires) {
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

int main() {
    ifstream source("release/test01/top_primitive.v");
    
    vector<pair<int, string>> inputs;
    vector<pair<int, string>> outputs;

    map<string, wire> wires;
    string line;
    while (getline(source, line, ';')) {
        //cout << line << '\n';
        line = trim(line);
        if (line.substr(0, 6) == "module") {
            continue;
        }
        if (line.substr(0, 7) == "endmodule") {
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
        if (line.substr(0, 6) == "wire") {
            process_wird(line, wires);
        }
    }
    for (string in : inputs) {
        cout << in << '\n';
    }
    for (string out : outputs) {
        cout << out << '\n';
    }
}
