#pragma once
#include <string>
#include <vector>
#include "moves.h"
#include "piece.h"

using std::string;
using std::vector;

namespace Graphene {

// --------------------------------------------------------------------
//  Data providers
// --------------------------------------------------------------------

struct Game
{
  vector<Move> moves;
  Piece winner;
};

class DataProvider // non-owning adapter
{
  vector<Game> & games;

public:
  DataProvider(vector<Game> & games) : games(games) {}
  bool open(string file);

private:
  bool open_dat(string file);
};

// --------------------------------------------------------------------
//  Tuners
// --------------------------------------------------------------------

class TunerPatterns
{
  vector<Game> games;

public:
  bool open(string file) { return DataProvider(games).open(file); }
  size_t size() const { return games.size(); }
  void start();
};

}
