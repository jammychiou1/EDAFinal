#ifndef __CONVERTER_H__
#define __CONVERTER_H__

#include <fstream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "simulator.h"
#include "solver.h"

// TODO: def and use join
class Converter {
 private:
  void m_write_prologue_block(const std::map<std::string, int> &widths,
                              std::string type);

  std::map<std::string, int> m_input_widths;
  std::map<std::string, int> m_output_widths;
  std::ofstream m_file;

 public:
  Converter(std::string output_path, std::map<std::string, int> input_widths,
            std::map<std::string, int> output_widths);

  void write_prologue();

  void write_output_cases(std::string output_name, std::string control_input,
                          const std::map<long long, std::string> &expressions);

  void write_output(std::string output_name, const std::string &expression);

  void write_epilogue();
};

#endif  // __CONVERTER_H__
