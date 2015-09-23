#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/genetic.h"

genotype galloc(void) {
  genotype buffer;
  buffer = (genotype)calloc(MAX_GENOTYPE_SIZE, sizeof(genotype_t));

  /*|  Transition  |  column  |  row  | */
  buffer[0] = ((rand() % 7) + 3) << 16;
  //buffer[0]  = (3 << 16);                             // 遷移回数
  buffer[0] += (DEFAULT_UNIT_SIZE << 8);              // 隠れ層サイズ
  buffer[0] += (DEFAULT_GENOTYPE_SIZE - HEADER_SIZE); // 遺伝型サイズ

  return buffer;
}

void ginit(genotype gtype) {
  int i, j, shift_size;
  const int column = (gtype[0] >> 8) & 0xFF;
  extern int alpha_size, thr_size, w_size;

  for(i = 0; i < column + 2; i++) {
    shift_size = (column + 1 - i) * 4;

    gtype[1] += (rand() % (RANGE + 1)) << shift_size;
    gtype[2] += (rand() % alpha_size) << shift_size;
    gtype[3] += (rand() % 2)          << shift_size;
    gtype[4] += (rand() % thr_size)   << shift_size;
  }
  // 近傍接続荷重
  for(i = 0; i < INTERNAL_SIZE; i++) {
    for(j = 0; j < column + 2; j++) {
      shift_size = (column + 1 - j) * 4;
      gtype[5 + i] += (rand() % w_size) << shift_size;
    }
  }
  // 外部接続荷重
  for(i = 0; i < column + 2; i++) {
    shift_size = (column + 1 - i) * 4;
    gtype[5 + INTERNAL_SIZE] += (rand() % w_size) << shift_size;
  }
  // 隠れ層接続荷重
  for(i = 0; i < column; i++) {
    for(j = 0; j < column + 2; j++) {
      shift_size = (column + 1 - j) * 4;
      gtype[5 + INTERNAL_SIZE + EXTERNAL_INPUT + i]
        += (rand() % w_size) << shift_size;
    }
  }
}
