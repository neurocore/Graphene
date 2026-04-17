#include <string>
#include "board.h"
#include "utils.h"

using namespace std;

namespace Graphene {

Board::Board(int size) : sz(size)
{
  init();
}

Board::Board(const Board & B)
{
  sz = B.sz;
  stm = B.stm;

  for (int r = 1; r <= sz; r++)
  {
    for (int f = 1; f <= sz; f++)
    {
      const SQ sq = to_sq(r, f);
      el[sq] = B.el[sq];
      parent[sq] = B.parent[sq];
    }
  }
}

void Board::init()
{
  const int M = sz + 1;
  stm = 0;

  for (SQ i = (SQ)0; i < MaxSQ; ++i)
  {
    el[i] = Nop;
    parent[i] = i;
  }

  for (int i = 1; i <= sz; i++)
  {
    place_softly(to_sq(0, i), Red);
    place_softly(to_sq(M, i), Red);
    place_softly(to_sq(i, 0), Blue);
    place_softly(to_sq(i, M), Blue);
  }
}

void Board::print() const
{
  string pad = " ";
  
  log("{}   ", pad);
  for (int f = 0; f < sz; f++)
    log("{} ", (char)('A' + f));
  log("\n");

  for (int r = 1; r <= sz; r++)
  {
    log("{}{:2d}  ", pad, r);
    for (int f = 1; f <= sz; f++)
    {
      Piece p = el[to_sq(r, f)];
      if (p == Red)  log("\033[31mX\033[0m ");
      if (p == Blue) log("\033[36mO\033[0m ");
      else           log(". ");
    }
    log("\n");
    pad += " ";
  }
  log("\n");
}

void Board::place(SQ sq, Piece p)
{
  // (r-1, f), (r-1, f+1), (r, f-1), (r, f+1), (r+1, f-1), (r+1, f)
  static const int offsets[] = { -32, -31, -1, +1, +31, +32 };

  el[sq] = p;
  
  for (int offset : offsets)
    if (el[sq + offset] == p)
      unite(sq, sq + offset);
}

bool Board::is_win(Piece p)
{
  const int M = sz + 1;

  switch (p)
  {
    case Red:
    {
      const SQ Top    = to_sq(0, 1);
      const SQ Bottom = to_sq(M, 1);
      return find(Top) == find(Bottom);
    }

    case Blue:
    {
      const SQ Left   = to_sq(1, 0);
      const SQ Right  = to_sq(1, M);
      return find(Left) == find(Right);
    }
  }
  return false;
}

SQ Board::find(SQ sq)
{
  if (parent[sq] != sq)
    parent[sq] = find(parent[sq]);
  return parent[sq];
}

void Board::unite(SQ a, SQ b)
{
  SQ ra = find(a);
  SQ rb = find(b);
  if (ra == rb) return; // same set
  if (ra > rb) std::swap(ra, rb);
  parent[ra] = rb;
}

void Board::place_softly(SQ sq, Piece p)
{
  static const int offsets[] = { -32, -31, -1, +1, +31, +32 };

  el[sq] = p;

  for (int offset : offsets)
    if (sq + offset < MaxSQ
    &&  el[sq + offset] == p)
      unite(sq, sq + offset);
}

}
