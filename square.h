#pragma once
#include <string>
#include <format>
#include <charconv>
#include "consts.h"
#include "types.h"

namespace Graphene {

// Actual board must contain sentinel stones
//  placed on edge sides to represent mailbox

enum SQ : u16
{
  A1 = 0x21, B1, C1, D1, // ..., T1
  A2 = 0x41, B2, C2, D2,

  // and so on

  None = MaxSQ - 1
};

using Move = SQ;

INLINE SQ to_sq(int r, int f) { return static_cast<SQ>((r << 5) + f); }
static SQ to_sq(std::string s)
{
  int r = 0;
  std::from_chars(s.data() + 1, s.data() + s.size(), r);
  return to_sq(r, 1 + s[0] - 'A');
}

INLINE int rank(SQ sq) { return sq >> 5; }
INLINE int file(SQ sq) { return sq & 31; }

INLINE std::string to_string(SQ sq)
{
  return std::string()
    + (char)('A' + file(sq) - 1)
    + std::to_string(rank(sq));
}

INLINE SQ operator + (SQ a, int i) { return static_cast<SQ>(+a + i); }
INLINE SQ operator - (SQ a, int i) { return static_cast<SQ>(+a - i); }

INLINE SQ & operator ++ (SQ & a) { a = static_cast<SQ>(a + 1); return a; }
INLINE SQ & operator -- (SQ & a) { a = static_cast<SQ>(a - 1); return a; }

}

template<>
struct std::formatter<Graphene::SQ> : std::formatter<std::string>
{
  auto format(const Graphene::SQ & sq, std::format_context & ctx) const
  {
    std::string str = to_string(sq);
    return std::formatter<std::string>::format(str, ctx);
  }
};
