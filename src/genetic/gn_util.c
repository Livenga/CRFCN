#include <stdio.h>
#include <stdlib.h>
#include "../../include/genetic.h"

void free_genotype(genotype *gtype, const int population) {
  int i;
  for(i = 0; i < population; i++){
    free(gtype[i]); gtype[i] = NULL;
  }
}

unsigned int get_gate(const int column) {
  int i;
  unsigned int gate = 0;
  for(i = 0; i < column; i++)
    gate += 0xF << (4 * i);
  return gate;
}
