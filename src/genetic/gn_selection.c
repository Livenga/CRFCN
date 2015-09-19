#include <stdio.h>
#include <stdlib.h>
#include "../../include/genetic.h"

int numof_best_fitness(const double *fitness, const int population) {
  int i, num = 0;
  double maxof = fitness[0];

  for(i = 1; i < population; i++) {
    if(maxof < fitness[i]) {
      maxof = fitness[i]; num = i;
    }
  }
  return num;
}

int numof_roulette(const double *fitness, const int population) {
  int i, j;
  double F = 0.0, r, sumof;
  double p[POPULATION_SIZE], q[POPULATION_SIZE];

  // 染色体適応度の総和
  for(i = 0; i < population; i++)
    F += fitness[i];

  // 染色体の選択確率
  for(i = 0; i < population; i++)
    p[i] = fitness[i] / F;

  // 染色体の累積確率
  for(i = 0; i < population; i++) {
    sumof = 0.0;
    for(j = 0; j < i; j++)
      sumof += p[j];
    q[i] = sumof;
  }

  /* 乱数rが関係式を満たす
   * q[i] < r < q[i + 1]
   */

  r = (double)(rand() % 1000) / 1000.0;
  for(i = 0; i < population - 1; i++)
    if(q[i] < r && r < q[i + 1])
      return i + 1;

  return population + 1;
}
