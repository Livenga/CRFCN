#include <math.h>
#include "../../include/graphic.h"

double calc_fitness(const int width, const int height,
    double *t, double *o, double *w) {
  int i, j, pos;
  double sum_top = 0.0, sum_bottom;

  for(i = 0; i < height; i++) {
    for(j = 0; j < width; j++) {
      pos = i * width + j;
      sum_top    += w[pos] * fabs(255.0 * o[pos] - 255.0 * t[pos]);
      sum_bottom += w[pos];
    }
  }
  return 1.0 - (sum_top / (255.0 * sum_bottom));
}
