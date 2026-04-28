#pragma once
#include <algorithm>
#include "square.h"
#include "consts.h"
#include "moves.h"

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
  int order[MaxSQ];

  Board(int size = 13);
  Board(const Board & B);
  void init();

  SQ   find(SQ sq);
  void unite(SQ a, SQ b);

  void place(SQ sq, Piece p);
  bool is_win(int opp = 1);
  bool legal(Move move);
  void print() const;

  void make(Move move);
  void generate(Moves & moves) const;

private:
  void place_softly(SQ sq, Piece p);
};

}
