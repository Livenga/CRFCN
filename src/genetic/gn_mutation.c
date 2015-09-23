#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/genetic.h"

void gmutation(genotype gtype) {
  int i, point_x, value;
  const int row = (gtype[0] & 0xFF), column = (gtype[0] >> 8) & 0xFF;
  extern int alpha_size, w_size, thr_size;
  point_x = (rand() % (row - 1)) + 1;

  switch(point_x) {
    case 1: value = RANGE + 1;  break;
    case 2: value = alpha_size; break;
    case 3: value = 2;          break;
    case 4: value = thr_size;   break;
    default: value = w_size;    break;
  }

  unsigned int gate = 0x00;
  for(i = 0; i < column + 2; i++)
    gate += (value - 1) << (i * 4);
  gtype[point_x] = gate - gtype[point_x];
}

int gunit_mutation(genotype gtype) {
  int i, j, add, value, shift_size;
  const int column = (gtype[0] >> 8) & 0xFF, row = (gtype[0] & 0xFF);
  extern int alpha_size, thr_size, w_size;

  add = ((column + 2) >= MAX_UNIT_SIZE) ? -1 : ((column <= 3) ? 1 :
      (rand() % 2) == 0) ? 1 : -1;

  // 突然変異は中間ユニットのみ起こる
  if(add == 1) { // 増加
    gtype[0] += 0x000101;

    // 追加の場合は, 染色体の外側に遺伝子座として挿入
    for(i = 1; i < row; i++) {
      switch(i) {
        case 1 : value = RANGE + 1;  break;
        case 2 : value = alpha_size; break;
        case 3 : value = 2;          break;
        case 4 : value = thr_size;   break;
        default: value = w_size;     break;
      }
      gtype[i] = (gtype[i] << 4) + (rand() % value);
    } // for(i)

    gtype[i] = 0x00;

    for(j = 0; j < column + 3; j++) {
      shift_size = (column + 2 - j) * 4;
      gtype[i] += (rand() % w_size) << shift_size;
    } // for(j)
  }

  else if(add == -1) { // 減少
    int del_pos = column - (rand() % (column - 2));
    unsigned gate = get_gate(del_pos);
    genotype_t buffer;
   
    //printf("%d\n", del_pos);
    gtype[0] -= 0x000101;

    for(i = 1; i < row; i++) {
      buffer   = 0x00;
      buffer   = (gtype[i] - (gtype[i] & (0x0F << (del_pos * 4)))) & ~gate;
      buffer   = buffer >> 4;

      buffer   = buffer + (gtype[i] & gate);
      gtype[i] = buffer;
    }
    for(i = 13 + (column - del_pos); i < row; i++)
      gtype[i] = gtype[i + 1];
    gtype[row] = 0x00;
  }

  return add;
}
#if 0
void gmutation(genotype gtype) {
  int point_x, point_y, value;
  unsigned int temp_value;

  const int column = (gtype[0] >> 8) & 0xFF, row = (gtype[0] & 0xFF);
  extern int alpha_size, thr_size, w_size;

  point_x = (rand() % column);
  point_y = (rand() % (row - 1)) + 1;

  switch(point_y) {
    case 1: value = RANGE + 1;  break;
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
        case 1 : value = RANGE + 1;  break;
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
#endif
