#pragma once
#include "board.h"
#include "timer.h"

namespace Graphene {

class Solver
{
protected:
  mutable bool thinking = false;
  mutable bool infinite = false;
  mutable bool verbose = true;

public:
  Solver() {}

  virtual bool is_solver() { return 0; }
  virtual void new_game() {}
  virtual Move get_move(Timestamp start, MS movetime) = 0;
  virtual void print_stats() const {}
  void stop() { thinking = false; }
  void set_analysis(bool val) { infinite = val; }
  void set_verbosity(bool val) { verbose = val; }
};

class Reader : public Solver
{
public:
  Reader() {}
  Move get_move(Timestamp start, MS movetime) override { return None; }
};

}
