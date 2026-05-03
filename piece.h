#pragma once
#include <string>
#include <format>
#include "types.h"

namespace Graphene {

enum Piece : u8 { Red, Blue, Nop };

INLINE Piece operator ~ (Piece p) { return static_cast<Piece>((int)p ^ 1); }
INLINE Piece operator ^ (Piece p, int o) { return static_cast<Piece>((int)p ^ o); }
INLINE Piece & operator ^= (Piece & p, int o) { p = p ^ o; return p; }
INLINE std::string to_string(Piece p) { return std::string() + "xo."[p]; }

}

template<>
struct std::formatter<Graphene::Piece> : std::formatter<std::string>
{
  auto format(const Graphene::Piece & p, std::format_context & ctx) const
  {
    std::string str = to_string(p);
    return std::formatter<std::string>::format(str, ctx);
  }
};
