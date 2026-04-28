#include <iostream>
#include <algorithm>
#include "learning.h"
#include "engine.h"
#include "utils.h"

using namespace std;

namespace Graphene {

Engine::Engine()
{
  S = new SolverMCTS(NodesMax);
  cout.sync_with_stdio(false);
  cerr.sync_with_stdio(false);
  cin.sync_with_stdio(false);
}

Engine::~Engine()
{
  delete S;
}

void Engine::start()
{
  new_game();

  bool success = true;
  while (success)
  {
    string str;
    getline(cin, str);
    move_start = Clock::now();

    if (str.length() > 0)
    {
      success = parse(str);
    }
  }
}

void Engine::response(std::string result)
{
  say<1>("= {}\n\n", result);

  //game_time -= elapsed(move_start);
}

void Engine::error(std::string message)
{
  say<1>("? {}\n\n", message);
}

bool Engine::parse(string str)
{
  static const string cmds[] =
  {
    "protocol_version", "name", "version", "list_commands", "known_command",
    "boardsize", "clear_board", "quit", "play", "genmove",
    //"time_settings", "time_left", "final_score", "showboard",
  };

  string cmd = cut(str);

  int id = parse_int(cmd, -1); // optional
  if (id >= 0) cmd = cut(str);

  if (cmd == "protocol_version")
  {
    response("2");
  }
  else if (cmd == "name")
  {
    response("Graphene");
  }
  else if (cmd == "version")
  {
    response("0.1");
  }
  else if (cmd == "list_commands")
  {
    response("");

    for (auto cmd : cmds)
      say("{}\n", cmd);
    say<1>("\n");
  }
  else if (cmd == "known_command")
  {
    string arg = cut(str);
    auto it = find(begin(cmds), end(cmds), arg);
    response(it == end(cmds) ? "false" : "true");
  }
  else if (cmd == "boardsize")
  {
    string arg = cut(str);
    int n = parse_int(arg);

    if (n < 4 || n > 19) error("unacceptable size");
    else
    {
      // TODO: set
    }
  }
  else if (cmd == "clear_board")
  {
    new_game();
  }
  else if (cmd == "quit")
  {
    return false;
  }
  else if (cmd == "play")
  {
    do
    {
      string color = cut(str);
      Piece p = parse_player(color);

      if (p == Nop)
      {
        error("unsupported color");
        break;
      }

      string move = cut(str);
      SQ sq = to_sq(move);

      if (!do_move(sq))
      {
        error("invalid move");
        break;
      }
    }
    while(false);
  }
  else if (cmd == "genmove")
  {
    string color = cut(str);
    Piece p = parse_player(color);

    go(p, move_time);
  }
  else if (cmd == "go")
  {
    string time = cut(str);
    MS ms = parse_int(time, TimeInc);

    go((Piece)B.stm, ms);
  }
  else if (cmd == "time_settings")
  {
    TimeSettings time;
    string main_time = cut(str);
    time.main_time = parse_int(main_time);

    string byo_yomi_time = cut(str);
    time.byo_yomi_time = parse_int(byo_yomi_time);

    string byo_yomi_stones = cut(str);
    time.byo_yomi_stones = parse_int(byo_yomi_stones);

    S->set(time);
  }
  else if (cmd == "time_left")
  {
    do
    {
      string color = cut(str);
      Piece p = parse_player(color);

      if (p == Nop)
      {
        error("unsupported color");
        break;
      }

      string time = cut(str);
      move_time = parse_int(time);
    }
    while(false);
  }
  else if (cmd == "final_score")
  {
  }
  else if (cmd == "showboard")
  {
  }
  else if (cmd == "learn") [[unlikely]]
  {
    do
    {
      string type = cut(str);
      string file = cut(str);

      if (type.empty()) { log("Format 'learn [type] [file]'\n"); break; }
      if (file.empty()) { log("Missing file to learn from\n"); break; }

      if (type == "patterns") learn_patterns(file);
      else
      {
        log("Unknown learning type\n");
        break;
      }
    }
    while(false);
  }
  else
  {
    error("unknown command");
  }

  return true;
}

Piece Engine::parse_player(std::string str)
{
  string color = cut(str);
  if (color == "b" || color == "black" || color == "red")  return Red;
  if (color == "w" || color == "white" || color == "blue") return Blue;
  return Nop;
}

void Engine::new_game()
{
  move_time = TimeDef;

  Board B0;
  S->set(B0);
  S->new_game();
}

bool Engine::do_move(Move move)
{
  if (!B.legal(move)) return false;

  B.make(move);
  return true;
}

void Engine::go(Piece p, MS movetime)
{
  S->set(B);
  Move move = S->get_move(move_start, movetime);
  response(to_string(move));

  B.make(move);
  B.print();

  S->print_stats();
}

void Engine::learn_patterns(std::string file)
{
  vector<Game> games;
  DataProvider(games).open(file);

  log("Loaded {} games\n", games.size());
}

}
