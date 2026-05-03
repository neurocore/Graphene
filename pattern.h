#pragma once
#include <algorithm>
#include <string>
#include <format>
#include <vector>
#include "square.h"
#include "piece.h"
#include "utils.h"

using std::string;
using std::format;
using std::vector;

namespace Graphene {

// (-1,0), (-1,1), (0,-1), (0,1), (1,-1), (1,0)
static const int p6_offsets[] = { -32, -31, -1, +1, +31, +32 };

// (-1,0), (-1,1), (0,-1), (0,1), (1,-1), (1,0)
// (-2,1), (-1,2), (1,1), (2,-1), (1,-2), (-1,-1)
static const int p12_offsets[] = { -32, -31, -1, +1, +31, +32, -63, -33, -30, 30, 33, 63 };

struct alignas(16) Pattern
{
  u32 key;
  float score;
  int appeared;
  int played;
};

extern Pattern p6[729];
extern Pattern p12[1 << 12];

// |

extern string p6_str(u32 key);
extern void p6_stats();

extern void p6_load();
extern void p6_save(string file);

}
