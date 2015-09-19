#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/genetic.h"

unsigned int get_gate(const int column) {
  int i;
  unsigned int gate = 0;
  for(i = 0; i < column; i++)
    gate += 0xF << 4;
  return gate;
}

void gcrossover(genotype *ch_gtype, genotype pr_gtype1, genotype pr_gtype2,
    const int population) {
  const int row = ((pr_gtype1[0] & 0x00FF) > (pr_gtype2[0] & 0x00FF)) ?
    (pr_gtype2[0] & 0x00FF) : (pr_gtype1[0] & 0x00FF);
  const int column = ((pr_gtype1[0] & 0x00FF00) > (pr_gtype2[0] & 0x00FF00)) ?
    ((pr_gtype2[0] & 0x00FF00) >> 8) : ((pr_gtype1[0] & 0x00FF00) >> 8);

  int i, pop, div_x, div_y;
  unsigned int gate;

  for(pop = 2; pop < population; pop++) {
    ch_gtype[pop][0] =
      (pr_gtype1[0] & 0x00FFFF) > (pr_gtype2[0] & 0x00FFFF) ?
      pr_gtype2[0] : pr_gtype1[0];

    div_x = (rand() % column);
    div_y = (rand() % row);

    gate  = get_gate(div_x);
    for(i = 1; i < div_y; i++)
      ch_gtype[pop][i] = (pr_gtype1[i] & gate) + (pr_gtype2[i] & ~gate);

    for(; i < row; i++) {
      ch_gtype[pop][i] = (pr_gtype1[i] & ~gate) + (pr_gtype2[i] & gate);
    }

    // 突然変異とか...
  }
}
