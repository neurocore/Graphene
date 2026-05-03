#pragma once
#include <string>
#include "solver_mcts.h"

namespace Graphene {

class Engine
{
  SolverMCTS * S;
  Board B;
  MS move_time;
  Timestamp move_start;

public:
  Engine();
  ~Engine();

  void start();
  bool parse(std::string str);
  Piece parse_player(std::string str);
  void response(std::string result = {});
  void error(std::string message = {});

  // commands
  void new_game();
  void set_pos(std::string fen, Moves moves);
  bool do_move(Move mv);
  void go(Piece p, MS movetime);
  void fen();

  // learning
  void learn_patterns(std::string file);
  void save_patterns(std::string file);
};

}
