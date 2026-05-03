#include <fstream>
#include "pattern.h"

using namespace std;

namespace Graphene {

Pattern p6[729];
Pattern p12[1 << 12];

string p6_str(u32 key)
{
  Piece p[6];
  for (int i = 0; i < 6; i++)
  {
    p[i] = (Piece)(key % 3);
    key /= 3;
  }

  string str;
  str  = format(" {} {}\n", p[0], p[1]);
  str += format("{} _ {}\n", p[2], p[3]);
  str += format(" {} {}\n", p[4], p[5]);
  return str;
}

void p6_stats()
{
  vector<Pattern> patterns;
  for (u32 i = 0; i < 729; i++)
  {
    p6[i].key = i;
    patterns.push_back(p6[i]);
  }

  std::sort(patterns.begin(), patterns.end(), [](const auto & a, const auto & b)
  {
    //return a.score > b.score; // better score
    return a.played * b.appeared > b.played * a.appeared; // better rate
  });

  for (int k = 0; k < 30; k++)
  {
    const Pattern & p = patterns[k];
    const float rate = 100.f * p.played / p.appeared;

    log("{}\n", p6_str(p.key));
    log("   score = {}\n", p.score);
    log(" percent = {:.4f}%\n", rate);
    log("  played = {} / {}\n\n", p.played, p.appeared);
  }
}

void p6_load()
{
  ifstream fin("p6.dat");
  for (u32 i = 0; i < 729; i++)
  {
    string str;
    getline(fin, str);

    auto parts = split(str);
    if (parts.size() != 3) break;

    p6[i].score = (float)parse_double(parts[0]);
    p6[i].played = parse_int(parts[1]);
    p6[i].appeared = parse_int(parts[2]);
  }
}

void p6_save(string file)
{
  ofstream fout(file);
  for (u32 i = 0; i < 729; i++)
  {
    fout << format("{} {} {}\n", p6[i].score, p6[i].played, p6[i].appeared);
  }
  fout.close();
}

}
