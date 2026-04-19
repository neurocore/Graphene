#pragma once
#include <random>
#include "board.h"
#include "timer.h"

namespace Graphene {

using Idx = u32;
enum : u32 { Null, Root, Empty };
enum WDL : u8 { NonTerminal, Win, Draw, Lose };

const float wdl_value[] = { 0.f, 1.f, 0.f, -1.f };
const float Utc_C = 1.41;

struct alignas(64) Node
{
  u32 visits = 5;
  float value = 0.f;

  float prior_prob = 0.f; // P(s, a)
  float node_value = 0.f; // V(s) from NN

  Idx parent_idx = 0;
  Idx child_start_idx = 0; // indexing in separate
  Idx children_count = 0;  //  table of children

  u64 hash_key = 0ull; // for nodes recycling in TT

  Move move = None;
  WDL terminal = NonTerminal;
};


class SolverMCTS
{
  Board B, B0;
  Node * N;

  Idx nodes_ptr = Empty;
  Idx nodes_max = 100'000;

  std::minstd_rand gen;
  std::uniform_real_distribution<double> dist;

public:
  SolverMCTS(u32 nodes_max);
  ~SolverMCTS();
  u32 nodes_used() const { return nodes_ptr; }

  Idx allocate_nodes(int cnt);
  void print_stats() const;

  WDL get_wdl();

  bool is_terminal(Idx node);
  bool is_expanded(Idx node) const;
  float get_uct(Idx node) const;

  Idx get_child(Idx node, Move move);
  Idx expand(Idx node);

  Move get_move(Timestamp start, MS movetime);
  void mcts();
  Idx tree_policy(Idx node);
  Idx get_best_child(Idx node);
  Idx get_most_visited(Idx node);
  float rollout(Idx node);
  void backprop(Idx node, float reward);
};

}
