#ifndef __PARSER_H__
#define __PARSER_H__

#include <cassert>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "circuit.h"

// TODO: Faster parsing.
class Parser {
 private:
  void process_input(std::string line);
  void process_output(std::string line);
  void process_wire(std::string line);
  void process_gate(std::string line);

  // std::set<std::string> inputs;
  // std::set<std::string> outputs;

  Circuit circuit;

  static std::pair<Type, std::string> identify_gate(std::string line);
  static std::pair<int, std::string> process_word_desc(std::string line);

 public:
  Circuit parse(const std::string &file_name);
};

#endif  // __PARSER_H__
