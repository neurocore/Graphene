#pragma once
#include <algorithm>
#include "square.h"
#include "consts.h"
#include "moves.h"
#include "piece.h"

namespace Graphene {

// Used disjoint set union (DSU) for
//  considering connections

struct Board
{
  int sz;
  Piece stm = Red;
  Piece el[MaxSQ];
  SQ parent[MaxSQ];
  int order[MaxSQ];

  Board(int size = 9);
  Board(const Board & B);
  void init();

  SQ   find(SQ sq);
  void unite(SQ a, SQ b);

  void place(SQ sq, Piece p);
  bool is_win(int opp = 1);
  bool legal(Move move);
  void print(SQ move = None) const;
  bool in_board(SQ sq) const;
  u32  extract_p6(SQ sq) const;
  u32  extract_p12(SQ sq) const;

  void make(Move move);
  void generate(Moves & moves) const;

private:
  void place_softly(SQ sq, Piece p);
};

}
