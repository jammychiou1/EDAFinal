#include "parser.h"

#include <algorithm>
#include <fstream>
#include <iostream>

#include "string_util.h"

using namespace std;

const map<string, Type> TYPE_KEYWORDS = {
    {"and", AND}, {"or", OR},   {"nand", NAND}, {"nor", NOR},
    {"not", NOT}, {"buf", BUF}, {"xor", XOR},   {"xnor", XNOR}};

const string bit_0 = "1\'b0";
const string bit_1 = "1\'b1";

const string module_keyword = "module";
const string endmodule_keyword = "endmodule";
const string input_keyword = "input";
const string output_keyword = "output";
const string wire_keyword = "wire";

Circuit Parser::parse(const string &file_name) {
  circuit.def_const(bit_0, -1, 0);
  circuit.def_const(bit_1, -1, 1);

  ifstream file(file_name);
  if (!file) {
    cerr << "Error opening file " << file_name << endl;
    return {};
  }
  string line;
  while (getline(file, line, ';')) {
    line = trim(line);
    if (starts_with(line, module_keyword)) {
      continue;
    }
    if (starts_with(line, endmodule_keyword)) {
      circuit.commit();
      // Circuit result = std::move(circuit);
      return std::move(circuit);
    }
    if (starts_with(line, input_keyword)) {
      process_input(line);
      continue;
    }
    if (starts_with(line, output_keyword)) {
      process_output(line);
      continue;
    }
    if (starts_with(line, wire_keyword)) {
      process_wire(line);
      continue;
    }
    process_gate(line);
  }
  assert(false);
}

pair<int, string> Parser::process_word_desc(string line) {
  if (line.front() == '[') {
    size_t right = line.find("]");
    string word_desc = slice(line, 1, right);
    string remain = trim(slice(line, right + 1, line.size()));
    vector<string> result = split(word_desc, ":");
    assert(result.size() == 2);
    int l = stoi(result[0]);
    int r = stoi(result[1]);
    assert(r == 0);
    return {l + 1, remain};
  }
  return {-1, line};
}

// TODO: Clean up duplicated code.
void Parser::process_input(string line) {
  line = trim(slice(line, input_keyword.size(), line.size()));
  auto [width, remain] = process_word_desc(line);

  vector<string> names = split(remain, ",");
  for_each(names.begin(), names.end(), [](string &s) { s = trim(s); });

  for (auto name : names) {
    circuit.def_input(name, width);
  }
}

void Parser::process_output(string line) {
  line = trim(slice(line, output_keyword.size(), line.size()));
  auto [width, remain] = process_word_desc(line);

  vector<string> names = split(remain, ",");
  for_each(names.begin(), names.end(), [](string &s) { s = trim(s); });

  for (auto name : names) {
    circuit.def_output(name, width);
  }
}

void Parser::process_wire(string line) {
  line = trim(slice(line, wire_keyword.size(), line.size()));
  auto [width, remain] = process_word_desc(line);

  vector<string> names = split(remain, ",");
  for_each(names.begin(), names.end(), [](string &s) { s = trim(s); });

  for (string name : names) {
    circuit.add_word(name, width);
  }
}

pair<Type, string> Parser::identify_gate(string line) {
  for (auto [keyword, type] : TYPE_KEYWORDS) {
    int n = keyword.size();
    if (starts_with(line, keyword)) {
      string remain = trim(line.substr(n));
      return {type, remain};
    }
  }
  cerr << "Error line: " << line << endl;
  assert(false);
}

void Parser::process_gate(string line) {
  auto [type, remain] = identify_gate(line);
  size_t left = remain.find("(");
  string name = trim(slice(remain, 0, left));

  string params = trim(slice(remain, left, remain.size()));
  assert(params.front() == '(');
  assert(params.back() == ')');
  params = trim(slice(params, 1, params.size() - 1));

  vector<string> names = split(params, ",");
  for_each(names.begin(), names.end(), [](string &s) { s = trim(s); });

  string output_name = names[0];
  vector<string> input_names(names.begin() + 1, names.end());
  circuit.add_gate(name, type, output_name, input_names);
}
