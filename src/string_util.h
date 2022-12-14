#ifndef __STRING_UTIL_H__
#define __STRING_UTIL_H__

#include <string>
#include <vector>

std::string slice(const std::string &str, int l, int r);
std::string trim(const std::string &str,
                 const std::string &chars = " \f\n\r\t\v");
bool starts_with(const std::string &str, const std::string &prefix);
std::vector<std::string> split(const std::string &str,
                               const std::string &delim);

#endif // __STRING_UTIL_H__
