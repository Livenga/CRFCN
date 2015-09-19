#include <stdio.h>
#include <stdlib.h>
#include "../../include/genetic.h"

void free_genotype(genotype *gtype, const int population) {
  int i;
  for(i = 0; i < population; i++){
    free(gtype[i]); gtype[i] = NULL;
  }
}
