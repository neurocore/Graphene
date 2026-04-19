#pragma once
#include "square.h"
#include "types.h"

namespace Graphene {

struct Moves
{
  int cnt;
  Move arr[MaxSQ];

  void clear() { cnt = 0; }
  void push(Move move) { arr[cnt++] = move; }
  int  size() const { return cnt; }
  Move operator[] (int i) const { return arr[i]; }
};

}
