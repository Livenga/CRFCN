#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/genetic.h"

void gmutation(genotype gtype) {
  int point_x, point_y, value;
  unsigned int temp_value;

  const int column = (gtype[0] >> 8) & 0xFF, row = (gtype[0] & 0xFF);
  extern int alpha_size, thr_size, w_size;

  point_x = (rand() % column);
  point_y = (rand() % (row - 1)) + 1;

  switch(point_y) {
    case 1: value = STEP;       break;
    case 2: value = alpha_size; break;
    case 3: value = 1;          break;
    case 4: value = thr_size;   break;
    default: value = w_size;    break;
  }

  temp_value = gtype[point_y] & (0x0F << (point_x * 4));
  gtype[point_y] -= gtype[point_y] & (0x0F << (point_x * 4));
  gtype[point_y] += (value << (point_x * 4)) - temp_value;
}
