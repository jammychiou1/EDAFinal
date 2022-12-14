#include "string_util.h"

#include <cassert>

using namespace std;

string slice(const string &str, int l, int r) {
  assert(0 <= l);
  assert(l <= r);
  assert(r <= str.size());
  return str.substr(l, r - l);
}

string trim(const string &str, const string &chars) {
  size_t l = str.find_first_not_of(chars);
  if (l == string::npos) {
    return "";
  }

  size_t r = str.find_last_not_of(chars);
  return slice(str, l, r + 1);
}

bool starts_with(const string &str, const string &prefix) {
  if (prefix.size() > str.size()) {
    return false;
  }
  return slice(str, 0, prefix.size()) == prefix;
}

vector<string> split(const string &str, const string &delim) {
  vector<string> result;
  size_t l = 0, r;
  while (true) {
    r = str.find(delim, l);
    if (r == string::npos) {
      result.push_back(slice(str, l, str.size()));
      break;
    }
    l = r + delim.size();
  }
  return result;
}
