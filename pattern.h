#pragma once

namespace Graphene {

struct alignas(16) Pattern
{
  float score;
  int appeared;
  int played;
};

Pattern p6[1 << 6];
Pattern p12[1 << 12];

}
