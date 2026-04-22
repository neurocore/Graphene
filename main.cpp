#include "utils.h"
#include "engine.h"

using namespace Graphene;

int main()
{
  say("Graphene v0.1 - hex playing engine\n");

  Engine E;
  E.start();

  /*SolverMCTS S(10'000'000);
  auto start = Clock::now();
  auto move = S.get_move(start, 10'000);

  S.print_stats();

  log("\nbest move: {}\n", move);
  log("nodes used: {}\n", S.nodes_used());*/

  return 0;
}
