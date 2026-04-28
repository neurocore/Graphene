#pragma once
#include <iostream>
#include <format>
#include <string>
#include <vector>
#include "types.h"

using Strings = std::vector<std::string>;

namespace Graphene {

INLINE std::string cut(std::string & str, std::string delim = " ")
{
  auto pos = str.find(delim);
  std::string part = str.substr(0, pos);
  str = pos == std::string::npos ? std::string()
      : str.substr(pos + delim.length(), str.length());
  return part;
}

INLINE void dry(std::string & str, std::string_view chars = "\n")
{
  size_t pos = str.length();
  for (char ch : chars)
  {
    while ((pos = str.rfind(ch, pos)) != std::string::npos)
    {
      str.erase(pos, 1);
    }
  }
}

static Strings split(const std::string & str, std::string delim = " ")
{
  Strings result;
  if (delim.empty()) return result;

  size_t start = 0, end;
  while ((end = str.find(delim, start)) != std::string::npos)
  {
    if (end != start)
      result.push_back(str.substr(start, end - start));
    start = end + delim.size();
  }
  result.push_back(str.substr(start));
  return result;
}

inline int parse_int(const std::string_view str, int def = 0, int base = 10)
{
  i64 result = def; // it can't correctly read hex int (8 chars)
  std::from_chars(str.data(), str.data() + str.size(), result, base);
  return static_cast<int>(result);
}

inline double parse_double(const std::string_view str, double def = 0.)
{
  double result = def;
  std::from_chars(str.data(), str.data() + str.size(), result);
  return result;
}

template<bool flush = false, typename... Args>
INLINE void say(std::format_string<Args...> fmt, Args&&... args)
{
  const auto str = std::format(fmt, std::forward<Args>(args)...);
  std::cout << str;
  if constexpr (flush) std::cout.flush();
}

template<typename... Args>
INLINE void log(std::format_string<Args...> fmt, Args&&... args)
{
  std::cerr << std::format(fmt, std::forward<Args>(args)...);
}

}
