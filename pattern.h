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
static int p6_offsets[] = { -32, -31, -1, +1, +31, +32 };

// (-1,0), (-1,1), (0,-1), (0,1), (1,-1), (1,0)
// (-2,1), (-1,2), (1,1), (2,-1), (1,-2), (-1,-1)
static int p12_offsets[] = { -32, -31, -1, +1, +31, +32, -63, -33, -30, 30, 33, 63 };

struct alignas(16) Pattern
{
  u32 key;
  float score;
  int appeared;
  int played;
};

// |

static Pattern p6[531441];
static Pattern p12[1 << 12];

static string p6_str(u32 key)
{
  Piece p[6];
  for (int i = 0; i < 6; i++)
  {
    p[i] = (Piece)(key % 3);
    key /= 3;
  }

  string str;
  str  = format(" {} {}\n", p[0], p[1]);
  str += format("{} _ {}\n", p[2], p[3]);
  str += format(" {} {}\n", p[4], p[5]);
  return str;
}

static void p6_stats()
{
  vector<Pattern> patterns;
  for (u32 i = 0; i < 531441; i++)
  {
    p6[i].key = i;
    patterns.push_back(p6[i]);
  }

  std::sort(patterns.begin(), patterns.end(), [](const auto & a, const auto & b)
  {
    return a.played * b.appeared > b.played * a.appeared; // better rate
  });

  for (int k = 0; k < 20; k++)
  {
    const Pattern & p = patterns[k];
    const float rate = 100.f * p.played / p.appeared;

    log("{}\n", p6_str(p.key));
    log("   score = {}\n", p.score);
    log(" percent = {:.4f}%\n", rate);
    log("  played = {} / {}\n\n", p.played, p.appeared);
  }
}

}
