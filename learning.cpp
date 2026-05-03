#include <cassert>
#include <fstream>
#include "learning.h"
#include "pattern.h"
#include "board.h"
#include "utils.h"

using namespace std;

namespace Graphene {

bool DataProvider::open(string file)
{
  auto parts = split(file, ".");
  if (parts.size() < 2)
  {
    log("Allowed only 'dat' extension\n");
    return false;
  }

  std::string ext = parts[parts.size() - 1];

  if (ext == "dat")
  {
    log("Reading dat...\n");
    if (!open_dat(file))
    {
      log("Error in reading file\n");
      return false;
    }
  }
  else
  {
    log("Unknown file format\n");
    return false;
  }

  if (games.size() < 1)
  {
    log("There is no any games in dataset\n");
    return false;
  }

  return true;
}

bool DataProvider::open_dat(string file)
{
  ifstream fin(file);
  if (!fin.is_open()) return false;

  string line;
  while (fin)
  {
    getline(fin, line);
    
    Board B;
    Game game;
    string move;
    while (!(move = cut(line)).empty())
    {
      const Move mv = to_sq(move);
      game.moves.push_back(mv);

      B.make(mv);
      if (B.is_win(1)) break;
    }

    if (B.is_win(1)) game.winner = B.stm ^ 1;
    else
    {
      // Game is not finished
      // Required deep analysis of
      //  virtual connections
      continue;
    }

    games.push_back(game);
  }
  return true;
}

void TunerPatterns::start()
{
  static const float win_score[] = { 0.3f, 1.0f };

  for (const auto & game : games)
  {
    Board B;
    for (Move move : game.moves)
    {
      const u32 key = B.extract_p6(move);
      p6[key].played++;
      p6[key].score += win_score[B.stm == game.winner];

      B.make(move);

      for (int r = 1; r <= B.sz; r++)
      {
        for (int f = 1; f <= B.sz; f++)
        {
          const SQ sq = to_sq(r, f);
          const u32 key = B.extract_p6(sq);

          p6[key].appeared++;
        }
      }
    }
  }

  p6_stats();
}

}
