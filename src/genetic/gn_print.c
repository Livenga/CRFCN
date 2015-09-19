#include <stdio.h>
#include <stdlib.h>
#include "../../include/genetic.h"

void printd(const char *str) {
  const char esc = 033;
  printf("%c[%dm%s%c[%dm", esc, 1, str, esc, 0);
}

const char *fname(const int f_num);
void draw_line(const int column);
void gprint(const int num, genotype gtype) {
  int i, j;
  const int row = gtype[0] & 0xFF, column = (gtype[0] >> 8) & 0xFF;
  extern double alpha[], thr[], w[];

  printf("[遺伝型番号] %d\n", num);
  printf("[サイズ : 遷移回数] %dx%d : %d\n", column, row, (gtype[0] >> 16) & 0xFF);

  int position_x;

  draw_line(column);
  printf("|           |");
  for(i = 0; i < column; i++) {
    printf("    H%2d    |", i);
  }
  printf("    IO     |"); printf("    EO     |\n");
  draw_line(column);

  printf("|  Funtion  |");
  for(i = 0; i < column + 2; i++) {
    position_x = (column + 1 - i) * 4;
    printf(" %9s |", fname((gtype[1] >> position_x) & 0x0F));
  }
  printf(" 0x%08X\n", gtype[1]);

  printf("|    Gain   |");
  for(i = 0; i < column + 2; i++) {
    position_x = (column + 1 - i) * 4;
    printf("   %+.2f   |", alpha[(gtype[2] >> position_x) & 0x0F]);
  }
  printf(" 0x%08X\n", gtype[2]);

  // Speed 
  printf("|   Speed   |");
  for(i = 0; i < column + 2; i++) {
    position_x = (column + 1 - i) * 4;
    printf("     %d     |", (gtype[3] >> position_x) & 0x0F);
  }
  printf(" 0x%08X\n", gtype[3]);


  printf("| Threshold |");
  for(i = 0; i < column + 2; i++) {
    position_x = (column + 1 - i) * 4;
    printf("   %+.2f   |", thr[(gtype[4] >> position_x) & 0x0F]);
  }
  printf(" 0x%08X\n", gtype[4]);
  draw_line(column);

  for(i = 0; i < INTERNAL_SIZE; i++) {
    printf("|    I%2d    |", i);
    for(j = 0; j < column + 2; j++) {
      position_x = (column + 1 - j) * 4;
      printf("   %+.2f   |", w[(gtype[5 + i] >> position_x) & 0x0F]);
    }
    printf(" 0x%08X\n", gtype[5 + i]);
  }
  draw_line(column);

  printf("|    EI0    |");
  for(i = 0; i < column + 2; i++) {
    position_x = (column + 1 - i) * 4;
    printf("   %+.2f   |", w[(gtype[5 + INTERNAL_SIZE] >> position_x) & 0x0F]);
  }
  printf(" 0x%08X\n", gtype[5 + INTERNAL_SIZE]);
  draw_line(column);

  for(i = 0; i < column; i++) {
    printf("|    H%2d    |", i);
    for(j = 0; j < column + 2; j++) {
      position_x = (column + 1 - j) * 4;
      printf("   %+.2f   |", w[(gtype[5 + INTERNAL_SIZE + EXTERNAL_INPUT + i] >> position_x) & 0x0F]);
    }
    printf(" 0x%08X\n", gtype[5 + INTERNAL_SIZE + EXTERNAL_INPUT + i]);
  }
  draw_line(column);
  putchar('\n');
}

void draw_line(const int column) {
  int i;
  printf("+-----------+");
  for(i = 0; i < column + 2; i++)
    printf("-----------+");
  putchar('\n');
}

const char *fname(const int f_num) {
  const char *msg;
  switch(f_num) {
    case SIGMOID:   msg = "Sigmoid";   break;
    case LINEAR:    msg = "Linear";    break;
    case PIECEWISE: msg = "Piecewise"; break;
    case THRESHOLD: msg = "Threshold"; break;
    case GAUSSIAN:  msg = "Gaussian";  break;
    case STEP:      msg = "Step";      break;
    case SUM:       msg = "Sum";       break;
    case MAX:       msg = "Max";       break;
    case MIN:       msg = "Min";       break;
    case '?': msg = "(nil)";           break;
    default:  msg = "(nil)";           break;
  }
  return msg;
}
