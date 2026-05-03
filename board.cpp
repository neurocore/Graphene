#include <string>
#include <cassert>
#include "board.h"
#include "utils.h"
#include "pattern.h"

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
      order[sq] = B.order[sq];
    }
  }
}

void Board::init()
{
  const int M = sz + 1;
  stm = Red;

  for (SQ i = (SQ)0; i < MaxSQ; ++i)
  {
    el[i] = Nop;
    parent[i] = i;
    order[i] = 0;
  }

  for (int i = 1; i <= sz; i++)
  {
    place_softly(to_sq(0, i), Red);
    place_softly(to_sq(M, i), Red);
    place_softly(to_sq(i, 0), Blue);
    place_softly(to_sq(i, M), Blue);
  }
}

void Board::print(SQ move) const
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
      const SQ sq = to_sq(r, f);
      const Piece p = el[sq];
      int col = p == Red ? 31 : p == Blue ? 36 : 90;
      if (sq == move) col += 60;
      log("\033[{}m{}\033[0m ", col, "XO."[p]);
    }
    log("\n");
    pad += " ";
  }
  log("\n");
}

bool Board::in_board(SQ sq) const
{
  return sq > 32 && sq < (sz + 1) * 33;
}

u32 Board::extract_p6(SQ sq) const
{
  assert(in_board(sq));

  int opp = stm == Blue;
  u16 key = 0u;

  for (int offset : p6_offsets)
  {
    Piece p = el[sq + offset];
    key *= 3;
    key += p == Nop ? Nop : p ^ opp;
  }
  return key;
}

u32 Board::extract_p12(SQ sq) const
{
  assert(in_board(sq));

  // ----------------------------------------
  //  TODO: not finished!
  // ----------------------------------------

  u16 key[3] = {0u, 0u, 0u};
  u16 unit = 1;

  for (int offset : p12_offsets)
  {
    if (sq + offset < 0
    &&  sq + offset >= (sz + 1) * 33)
      continue;

    Piece p = el[sq + offset];
    key[p] |= unit;
    unit  <<= 1;
  }
  return key[0] + (key[1] << 16);

  // ----------------------------------------
  //  TODO: not finished!
  // ----------------------------------------
}

void Board::place(SQ sq, Piece p)
{
  assert(in_board(sq));

  // (r-1, f), (r-1, f+1), (r, f-1), (r, f+1), (r+1, f-1), (r+1, f)
  static const int offsets[] = { -32, -31, -1, +1, +31, +32 };

  el[sq] = p;
  
  for (int offset : offsets)
    if (el[sq + offset] == p)
      unite(sq, sq + offset);
}

bool Board::is_win(int opp)
{
  const int M = sz + 1;

  switch (stm ^ opp)
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

bool Board::legal(Move move)
{
  const int r = rank(move);
  const int f = file(move);
  if (r < 1 || r > sz) return false;
  if (f < 1 || f > sz) return false;
  return el[move] == Nop;
}

SQ Board::find(SQ sq)
{
  while (parent[sq] != sq)
  {
    parent[sq] = parent[parent[sq]];
    sq = parent[sq];
  }
  return sq;
}

void Board::unite(SQ a, SQ b)
{
  const SQ ra = find(a);
  const SQ rb = find(b);
  if (ra == rb) return; // same set
  if (order[ra] < order[rb]) parent[ra] = rb;
  else if (order[ra] > order[rb]) parent[rb] = ra;
  else { parent[rb] = ra; order[ra]++; }
}

void Board::place_softly(SQ sq, Piece p)
{
  static const int offsets[] = { -32, -31, -1, +1, +31, +32 };
  const int max = (sz + 2) * 33;

  el[sq] = p;

  for (int offset : offsets)
    if (sq + offset >= 0
    &&  sq + offset < max
    &&  el[sq + offset] == p)
      unite(sq, sq + offset);
}

void Board::make(Move move)
{
  assert(move != None);
  assert(move < (sz + 1) * 33);
  if (move == None) return;
  place(move, stm);
  stm ^= 1;
}

void Board::generate(Moves & moves) const
{
  moves.clear();
  for (int r = 1; r <= sz; r++)
  {
    for (int f = 1; f <= sz; f++)
    {
      const SQ sq = to_sq(r, f);
      if (el[sq] == Nop) moves.push(sq);
    }
  }
}

}
