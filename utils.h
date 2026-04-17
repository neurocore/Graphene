#pragma once
#include <iostream>
#include <format>
#include "types.h"

namespace Graphene {

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
