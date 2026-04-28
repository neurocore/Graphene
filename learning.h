#pragma once
#include <string>
#include <vector>
#include "moves.h"

using std::string;
using std::vector;

namespace Graphene {

// --------------------------------------------------------------------
//  Data providers
// --------------------------------------------------------------------

struct Game
{
  vector<Move> moves;
  int result;
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


}
