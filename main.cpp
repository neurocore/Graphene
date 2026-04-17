#include "utils.h"
#include "board.h"

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

  return 0;
}
