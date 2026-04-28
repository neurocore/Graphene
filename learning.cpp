#include <fstream>
#include "learning.h"
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
    }

    if (B.is_win(1)) game.result = B.stm ^ 1;
    else
    {
      // Game is corrupted (expected win)
      continue;
    }

    games.push_back(game);
  }
  return true;
}

}
