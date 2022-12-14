#ifndef __SOLVER_H__
#define __SOLVER_H__

#include <functional>
#include <map>
#include <optional>
#include <utility>
#include <vector>

#include "circuit.h"
#include "math_util.h"

typedef std::vector<int> TermDesc;
typedef std::vector<std::pair<TermDesc, BigInt>> Formula;

class Solver {
 private:
  struct Word {
    int idx;
    int width;
    std::string name;

    Word(int idx, int width, std::string name)
        : idx(idx), width(width), name(name) {}
  };

  class WordLookup {
   private:
    std::vector<Word> m_words;
    std::map<std::string, int> m_name_to_idx;

   public:
    int count(int idx) { return 0 <= idx && idx < m_words.size(); }
    int count(std::string name) { return m_name_to_idx.count(name); }
    int get_num_words() { return m_words.size(); }

    void add_word(std::string name, int width) {
      int idx = get_num_words();
      m_words.emplace_back(idx, width, name);
      m_name_to_idx[name] = idx;
    }

    Word &get_word(int idx) { return m_words[idx]; }
    Word &get_word(std::string name) { return m_words[m_name_to_idx.at(name)]; }

    typedef std::vector<Word>::iterator Iterator;
    Iterator begin() { return m_words.begin(); }
    Iterator end() { return m_words.end(); }
  };

  struct Sample {
    std::vector<BigInt> inputs;
    std::vector<BigInt> outputs;
  };

  WordLookup m_input_infos;
  WordLookup m_output_infos;

  std::vector<Sample> m_samples;

 public:
  Solver(const std::map<std::string, int> &input_widths,
         const std::map<std::string, int> &output_widths);

  void load_data(int num_samples,
                 const std::map<std::string, std::vector<BigInt>> &inputs,
                 const std::map<std::string, std::vector<BigInt>> &outputs);

  std::optional<Formula> solve_output(std::string name);
  // std::optional<std::map<std::string, Formula>> solve();

  std::string format_term(const TermDesc &term, BigInt coeff = BigInt(1));
  std::string format_formula(const Formula &formula);
};

#endif  // __SOLVER_H__
