#ifndef __CONVERTER_H__
#define __CONVERTER_H__

#include <bits/stdc++.h>

class Converter {

public:
  Converter(std::vector<std::pair<std::string, int>> input_info,
            std::vector<std::pair<std::string, int>> output_info) {
    inputs = input_info;
    outputs = output_info;
    pre = false;
    start_cond = false;
  };
  ~Converter(){};
  void convert(std::string path,
               std::vector<std::map<std::vector<int>, __int128>>);

  void
  convert_control(std::string path,
                  std::vector<std::map<std::vector<int>, __int128>> result = {},
                  std::string control_input = "", int val = -1);

  void prewrite(std::ofstream &file);

  void inwrite(std::ofstream &file,
               std::vector<std::map<std::vector<int>, __int128>> result,
               std::string control_input = "", int val = -1);

  void postwrite(std::ofstream &file);

  std::vector<std::pair<std::string, int>> inputs;
  std::vector<std::pair<std::string, int>> outputs;

private:
  std::string generate_lines(std::vector<std::pair<std::string, int>> &,
                             std::string type);
  bool pre, start_cond;
};

#endif
