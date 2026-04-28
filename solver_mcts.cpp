#include <cassert>
#include <vector>
#include <utility>
#include "solver_mcts.h"
#include "moves.h"
#include "utils.h"

using namespace std;

namespace Graphene {

SolverMCTS::SolverMCTS(u32 nodes_max)
  : gen(random_device{}()), dist(0, 1)
  , nodes_max(nodes_max)
{
  N = new Node[nodes_max];
}

SolverMCTS::~SolverMCTS()
{
  delete[] N;
}

void SolverMCTS::new_game()
{
  // TODO
}

void SolverMCTS::set(const Board & board)
{
  B = board;
}

void SolverMCTS::set(const TimeSettings & time)
{
  this->time = time;
}

// --------------------------------------------
//   Utilities
// --------------------------------------------

Idx SolverMCTS::allocate_nodes(int cnt)
{
  Idx ptr = nodes_ptr;
  nodes_ptr += cnt;
  if (nodes_ptr > nodes_max) return Null;
  return ptr;
}

void SolverMCTS::print_stats() const
{
  using Stat = tuple<int, float, Move>;
  vector<Stat> stats;

  const Idx start = N[Root].child_start_idx;
  const Idx end = start + N[Root].children_count;

  for (Idx i = start; i < end; i++)
  {
    const float rate = N[i].value / N[i].visits;
    stats.push_back( make_tuple(N[i].visits, rate, N[i].move) );
  }

  sort(stats.begin(), stats.end(), [](const Stat & a, const Stat & b)
  {
    return get<0>(a) > get<0>(b);
  });

  for (int i = 0; i < 5; i++)
  {
    auto [visits, rate, move] = stats[i];
    log("{} - {:.5f} / {}\n", move, rate, visits);
  }

  log("\n");
  log(" best move: {}\n", get<2>(stats[0]));
  log("  win rate: {}\n", get<1>(stats[0]));
  log("nodes used: {}\n\n", nodes_used());
}

// --------------------------------------------
//   Trivial checks
// --------------------------------------------

WDL SolverMCTS::get_wdl()
{
  //if (B.is_draw()) return Draw; // no way
  if (B.is_win(1)) { log("{}\n", B.stm ? "Red" : "Blue"); B.print(); return Lose; }
  if (B.is_win(0)) { log("{}\n", B.stm ? "Blue" : "Red"); B.print(); return Win; }
  return NonTerminal;
}

bool SolverMCTS::is_terminal(Idx node)
{
  if (N[node].terminal) return true;

  const WDL wdl = get_wdl();

  N[node].terminal = wdl;
  return wdl;
}

bool SolverMCTS::is_expanded(Idx node) const
{
  return N[node].children_count > 0;
}

float SolverMCTS::get_uct(Idx node) const
{
  const int visits = N[node].visits;
  const Idx parent = N[node].parent_idx;
  assert(visits > 0);

  float parent_visits = parent ? N[parent].visits : 0.f;
  float winrate = N[node].value / N[node].visits;

  if (B.stm) winrate = -winrate;

  float uncertainty = std::log(parent_visits) / visits;
  return winrate + Utc_C * sqrt(uncertainty);
}

// --------------------------------------------
//   Working with nodes
// --------------------------------------------

Idx SolverMCTS::get_child(Idx node, Move move)
{
  const Idx start = N[node].child_start_idx;
  const Idx end = start + N[node].children_count;

  for (Idx i = start; i < end; i++)
  {
    if (N[i].move == move) return i;
  }
  return Null;
}

Idx SolverMCTS::expand(Idx node)
{
  Moves moves;
  B.generate(moves);
  const int size = moves.size();

  N[node].child_start_idx = allocate_nodes(size);
  N[node].children_count = size;

  if (!N[node].child_start_idx) return Null;

  const Idx start = N[node].child_start_idx;

  for (Idx i = start; i < start + size; i++)
  {
    N[i].parent_idx = node;
    N[i].move = moves[i - start];
    N[i].prior_prob = 0.f; //
    N[i].node_value = 0.f; //
  }
  B.make(N[start].move);
  //B.print();
  return start;
}

// --------------------------------------------
//   Parts of MCTS algorithm
// --------------------------------------------

Move SolverMCTS::get_move(Timestamp start, MS movetime)
{
  nodes_ptr = Empty;
  for (Idx i = 0; i < nodes_max; i++)
    N[i] = {};

  B.print();
  if (is_terminal(Root)) return None;

  while (nodes_ptr      < nodes_max
  &&     elapsed(start) < movetime)
  {
    mcts();
  }

  MS time = elapsed(start);
  log("Done - {} ms\n", time);
  Idx best = get_most_visited(Root);

  B0.make(N[best].move);
  return N[best].move;
}

void SolverMCTS::mcts()
{
  B = B0; // set at root
  const Idx curr = tree_policy(Root);
  if (!curr) return;

  const float reward = rollout(curr);
  backprop(curr, reward);
}

Idx SolverMCTS::tree_policy(Idx node)
{
  while (!is_terminal(node))
  {
    if (!is_expanded(node)) return expand(node);
    node = get_best_child(node);
  }
  return node;
}

Idx SolverMCTS::get_best_child(Idx node)
{
  int best = 0;
  float best_uct = -1e8;

  const Idx start = N[node].child_start_idx;
  const Idx end = start + N[node].children_count;

  for (int i = start; i < end; i++)
  {
    const float uct = get_uct(i);

    if (uct > best_uct)
    {
      best_uct = uct;
      best = i;
    }
  }
  B.make(N[best].move);
  return best;
}

Idx SolverMCTS::get_most_visited(Idx node)
{
  int best = 0;
  int best_visits = 0;

  const Idx start = N[node].child_start_idx;
  const Idx end = start + N[node].children_count;

  for (int i = start; i < end; i++)
  {
    if (N[i].visits > best_visits)
    {
      best_visits = N[i].visits;
      best = i;
    }
  }
  return best;
}

float SolverMCTS::rollout(Idx node)
{
  int stm = B.stm;

  WDL wdl;
  while ((wdl = get_wdl()) == NonTerminal)
  {
    Moves moves;
    B.generate(moves);

    int best = (int)(dist(gen) * moves.size());
    B.make(moves[best]);
  }

  return (B.stm ^ stm) ? wdl_value[wdl] : -wdl_value[wdl];
}

void SolverMCTS::backprop(Idx node, float reward)
{
  while (node)
  {
    N[node].value += reward;
    N[node].visits++;

    node = N[node].parent_idx;
    reward = -reward;
  }
}

}
