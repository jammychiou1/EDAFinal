#include "parser.h"

void
Parser::read(const string& path) {
    ifstream file(path.c_str());
    if (!file) {
        cerr << "Error opening file " << path << endl;
        return;
    }
    string line;
    while (getline(file, line, ';')) {
        line = Parser::trim(line);
        //cout << '<' << line << '>' << '\n';
        if (line.substr(0, 6) == "module") {
            firstLine = line;
            continue;
        }
        if (line.substr(0, 9) == "endmodule") {
            continue;
        }
        if (line.substr(0, 5) == "input") {
            process_input(line);
            continue;
        }
        if (line.substr(0, 6) == "output") {
            process_output(line);
            continue;
        }
        if (line.substr(0, 4) == "wire") {
            process_wire(line);
            continue;
        }
        process_gate(line);
    }
}

void 
Parser::dfsFanin(Based* ptr) const {
    if (ptr) {
        dfsFanin_helper(ptr, 0);
    }
    else {
        for (int i = 0; i < outputs.size(); ++i) {
            for (int j = 0; j < outputs[i]->outs.size(); ++j) {
                dfsFanin_helper(outputs[i]->outs[j], 0);
            }
        }
    }
}
string
Parser::trim(const string& s) {
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

pair<int, string> 
Parser::process_word_desc(string line) {
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

GateType 
Parser::identify_gate(string &line) {
    vector<string> types = {"and", "or", "nand", "nor", "not", "buf", "xor", "xnor"};   // index i is the same in enum
    for (int i = 0; i < types.size(); ++i) {
        int n = types[i].size();
        if (line.substr(0, n) == types[i]) {
            line = trim(line.substr(n));
            return GateType(i);
        }
    }
    assert(false);
}

void 
Parser::process_input(string line) {
    line = trim(line.substr(5));
    pair<int, string> result = process_word_desc(line);
    line = result.second;
    int width = result.first;

    line.push_back(',');
    string now;
    if (width == -1) {
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == ',') {
                const string name = trim(now);
                In* in = new In(name, width);
                //inputs.push_back({width, trim(now)});
                inputs.push_back(in);
                outputsMap[name] = in->ins[0];
                now = "";
                continue;
            }
            now.push_back(line[i]);
        }
    }
    else {
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == ',') {
                const string name = trim(now);
                In* in = new In(name, width);
                //inputs.push_back({width, trim(now)});
                inputs.push_back(in);
                for (int j = 0; j < width; ++j)
                    inputsMap[name + "[" + to_string(j) + "]"] = in->ins[j];
                now = "";
                continue;
            }
            now.push_back(line[i]);
        }
    }
}

void 
Parser::process_output(string line) {
    line = trim(line.substr(6));

    pair<int, string> result = process_word_desc(line);
    line = result.second;
    int width = result.first;

    line.push_back(',');
    string now;
    if (width == -1) {
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == ',') {
                const string name = trim(now);
                Out* out = new Out(name, width);    // will new Based* for width
                //outputs.push_back({width, trim(now)});
                outputs.push_back(out);
                outputsMap[name] = out->outs[0];
                now = "";
                continue;
            }
            now.push_back(line[i]);
        }
    }
    else {
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == ',') {
                const string name = trim(now);
                Out* out = new Out(name, width);
                //outputs.push_back({width, trim(now)});
                outputs.push_back(out);
                for (int j = 0; j < width; ++j)
                    outputsMap[name + "[" + to_string(j) + "]"] = out->outs[j];
                now = "";
                continue;
            }
            now.push_back(line[i]);
        }
    }
}

void 
Parser::process_wire(string line) {
    line = trim(line.substr(4));
    pair<int, string> result = process_word_desc(line);
    line = result.second;
    int width = result.first;

    line.push_back(',');
    string now;
    if (width == -1) {
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == ',') {
               // wires[trim(now)].input = nullptr;
                const string name = trim(now);
                wires[name] = new Based(name, NET);
                //wires[name]->addFanin(nullptr);
                auto it = outputsMap.find(name);
                //cout << "find name: " << name << endl;
                if (it != outputsMap.end()) {
                    //cout << "found in outputs" << endl;
                    it->second->addFanin(wires[now]);
                    wires[name]->addFanout(it->second);
                }
                it = inputsMap.find(name);
                if (it != inputsMap.end()) {
                    //cout << "found in inputs" << endl;
                    it->second->addFanout(wires[now]);
                    wires[name]->addFanin(it->second);
                }
                now = "";
                continue;
            }
            now.push_back(line[i]);
        }
    }
    else {
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == ',') {
                now = trim(now);
                for (int j = 0; j < width; j++) {
                    //wires[now + "[" + to_string(j) + "]"].input = nullptr;
                    const string name = now + "[" + to_string(j) + "]";
                    wires[name] = new Based(name, NET);
                    //wires[name]->addFanin(nullptr);
                    //cout << "find name: " << name << endl;
                    auto it = outputsMap.find(name);
                    if (it != outputsMap.end()) {
                        //cout << "found in outputs" << endl;
                        it->second->addFanin(wires[name]);
                        wires[name]->addFanout(it->second);
                    }
                    it = inputsMap.find(name);
                    if (it != inputsMap.end()) {
                        //cout << "found in inputs" << endl;
                        it->second->addFanout(wires[name]);
                        wires[name]->addFanin(it->second);
                    }
                }
                now = "";
                continue;
            }
            now.push_back(line[i]);
        }
    }
}

void 
Parser::process_gate(string line) {
    GateType gateType = identify_gate(line);
    size_t para = line.find("(");
    assert(para != string::npos);
    string name = line.substr(0, para);
    //cerr << name << '\n';
    name = Parser::trim(name);
    line = line.substr(para);
    assert(line[0] == '(');
    assert(line[line.size() - 1] == ')');
    line = line.substr(1, line.size() - 2) + ",";
    //cerr << line << '\n';
    string now;
    bool is_output = true;
    //Gate *gate = new Gate();
    Based* gate = new Based(name, GATE);
    gate->gateType = gateType;
    for (int i = 0; i < line.size(); i++) {
        if (line[i] == ',') {
            now = trim(now);
            assert(wires[now] != nullptr);
            if (is_output) {
                is_output = false;
                wires[now]->addFanin(gate);
                gate->addFanout(wires[now]);    //
                //cout << "add fanin: " << wires[now]->getName() << " " << name << endl;
            }
            else {
                wires[now]->addFanout(gate);
                gate->addFanin(wires[now]); //
                //cout << "add fanout: " << wires[now]->getName() << " " << name << endl;
            }
            now = "";
            continue;
        }
        now.push_back(line[i]);
    }
}


void
Based::addFanin(Based* ptr) {
    fanins.push_back(ptr);
}
void 
Based::addFanout(Based* ptr) {
    fanouts.push_back(ptr);
}

void 
Parser::dfsFanin_helper(Based* ptr, int depth) const {
    assert(ptr);
    for (int i = 0; i < depth; ++i) {
        cout << " ";
    }
    if (ptr->type == GATE) {
        cout << ptr->name << " ";
        cout << ptr->gateType << endl;
    }
    else 
        cout << ptr->name << endl;
    if (ptr->type == INPUT) 
        return;
    for (int i = 0; i < ptr->fanins.size(); ++i)
        dfsFanin_helper(ptr->fanins[i], depth+1);
}

void
Parser::strash() {
    globalRef ++;
    for (auto it = inputsMap.begin(); it != inputsMap.end(); ++it) {
        strash_helper(it->second);
    }
    cout << "Total reduce " << reducedNum << " equivalent gate" << endl;
}

void
Parser::strash_helper(Based* ptr) {
    if (ptr->ref == globalRef)
        return;
    ptr->ref = globalRef;
    if (ptr->type == GATE) { 
        if (hash.count(make_pair(ptr->fanins, ptr->gateType)) == 0) {
            hash[make_pair(ptr->fanins, ptr->gateType)] = ptr;
        }
        if (hash[make_pair(ptr->fanins, ptr->gateType)] != ptr) {
            // merge
            cout << "redundant: " << ptr->name << endl;
            reducedNum++;
            Based* oldPtr = hash[make_pair(ptr->fanins, ptr->gateType)];
            for (int i = 0; i < ptr->fanouts.size(); ++i) {
                oldPtr->addFanout(ptr->fanouts[i]);
                Based* outPtr = ptr->fanouts[i];
                for (int j = 0; j < outPtr->fanins.size(); ++j) {
                    if (outPtr->fanins[j] == ptr) {
                        outPtr->fanins[j] = oldPtr;
                        break;
                    }
                }
            }
            for (int i = 0; i < ptr->fanins.size(); ++i) {
                Based* inPtr = ptr->fanins[i];
                auto it = find(inPtr->fanouts.begin(), inPtr->fanouts.end(), ptr);
                assert(it != inPtr->fanouts.end());
                inPtr->fanouts.erase(it);
            }
        }
        
    }
    for (int i = 0; i < ptr->fanouts.size(); ++i)
        strash_helper(ptr->fanouts[i]);
}
/*
size_t
Parser::hashKey(vector<Based*> v, int type) const {
    size_t ret = 0;
    for (int i = 0; i < v.size(); ++i)
        ret ^= (size_t)v[i];
    return ret + type;
}
*/