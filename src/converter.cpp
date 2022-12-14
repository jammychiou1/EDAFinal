#include "converter.h"

using namespace std;

const string indent = "  ";

Converter::Converter(string output_path, map<string, int> input_widths,
                     map<string, int> output_widths) {
  m_input_widths = input_widths;
  m_output_widths = output_widths;
  m_file.open(output_path, ios_base::app);
};

void Converter::m_write_prologue_block(const map<string, int> &widths,
                                       string type) {
  for (auto [name, width] : widths) {
    m_file << indent << type << " ";
    if (width != -1) {
      m_file << "[" << width - 1 << ":0] ";
    }
    m_file << name << ";\n";
  }
}

void Converter::write_prologue() {
  stringstream parameters_ss;
  bool first = true;
  for (auto [name, width] : m_input_widths) {
    if (first) {
      parameters_ss << name;
      first = false;
    } else {
      parameters_ss << ", " << name;
    }
  }
  for (auto [name, width] : m_output_widths) {
    if (first) {
      parameters_ss << name;
      first = false;
    } else {
      parameters_ss << ", " << name;
    }
  }
  m_file << "module top(" << parameters_ss.str() << ");\n";

  m_write_prologue_block(m_input_widths, "input");
  m_write_prologue_block(m_output_widths, "output");
  m_write_prologue_block(m_input_widths, "wire");
  m_write_prologue_block(m_output_widths, "wire");
}

void Converter::write_output_cases(string output_name, string control_input,
                                   const map<long long, string> &expressions) {
  m_file << indent << "assign " << output_name << " =\n";
  for (auto [val, expression] : expressions) {
    m_file << indent << indent << control_input << " == " << val << " ? "
           << expression << " :\n";
  }
  m_file << indent << indent << "0; // should never happen\n";
}

void Converter::write_output(string output_name, const string &expression) {
  m_file << indent << "assign " << output_name << " = " << expression << ";\n";
}

void Converter::write_epilogue() { m_file << "endmodule\n"; }
