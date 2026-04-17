#pragma once
#include <algorithm>
#include "square.h"
#include "consts.h"

namespace Graphene {

enum Piece : u8 { Red, Blue, Nop };

// Used disjoint set union (DSU) for
//  considering connections

struct Board
{
  int sz;
  int stm = 0;
  Piece el[MaxSQ];
  SQ parent[MaxSQ];

  Board(int size = 11);
  Board(const Board & B);
  void init();
  void print() const;

  void place(SQ sq, Piece p);
  bool is_win(Piece p);
  SQ   find(SQ sq);
  void unite(SQ a, SQ b);

private:
  void place_softly(SQ sq, Piece p);
};

}
