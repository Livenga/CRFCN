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
    case 3: value = 2;          break;
    case 4: value = thr_size;   break;
    default: value = w_size;    break;
  }

  temp_value = gtype[point_y] & (0x0F << (point_x * 4));
  gtype[point_y] -= gtype[point_y] & (0x0F << (point_x * 4));
  gtype[point_y] += (value << (point_x * 4)) - temp_value;
}

void gunit_mutation(genotype gtype) {
  int i, j, add, value;
  const int column = (gtype[0] >> 8) & 0xFF, row = (gtype[0] & 0xFF);
  extern int alpha_size, thr_size, w_size;

  add = ((column + 2) >= MAX_UNIT_SIZE) ? -1 : (((column + 2) <= 5) ? 1 :
      (rand() % 2) == 0) ? 1 : -1;

  if(add == 1) { // 増加
    for(i = 1; i < row; i++) {
      switch(i) {
        case 1 : value = STEP;       break;
        case 2 : value = alpha_size; break;
        case 3 : value = 2;          break;
        case 4 : value = thr_size;   break;
        default: value = w_size;     break;
      }
      gtype[i] = (gtype[i] << 4) + (rand() % value);
    }

    gtype[row] = 0x00;
    int shift_size;
    for(i = 0; i < column + 3; i++) {
      shift_size = (column + 2 - i) * 4;
      gtype[row] += (rand() % w_size) << shift_size;
    }
    gtype[0] += 0x000101;
  }

  else if(add == -1) { // 減少
    for(i = 1; i < row + 1; i++)
      gtype[i] = gtype[i] >> 4;
    gtype[row + 1] = 0x00;
    gtype[0] -= 0x000101;
  }
}
