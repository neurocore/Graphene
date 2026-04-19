#include "utils.h"
#include "solver_mcts.h"

using namespace Graphene;

int main()
{
  say("Graphene v0.1 - hex playing engine\n");
  say("{}\n", (int)to_sq("A19"));

  Board B;
  B.print();

  say("R win: {}\n", B.is_win(Red));
  say("B win: {}\n\n", B.is_win(Blue));

  B.place(B2, Blue);
  for (int i = 1; i <= 11; i++)
    B.place(to_sq(i, 5), Red);
  B.print();

  say("R win: {}\n", B.is_win(Red));
  say("B win: {}\n\n", B.is_win(Blue));

  Solver * S = new SolverMCTS(10'000'000);
  auto start = Clock::now();
  auto move = S->get_move(start, 30'000);

  S->print_stats();

  log("\nbest move: {}\n", (int)move);
  //log("nodes used: {}\n", S->nodes_ptr);

  return 0;
}
