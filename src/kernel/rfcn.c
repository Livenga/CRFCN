#define KERNEL_FUNC
#include "include/genetic.h"

__kernel void rfcn(
    __global int *in_width,     // 横幅
    __global int *in_height,    // 縦幅
    __global genotype_t *gtype,  // 遺伝子型
    __global double *input,     // 入力
    __global double *in_output, // 内部入力
    __global double *ext_output // 外部入力
    ) {
  const int x = get_global_id(0), y = get_global_id(1);  // 位置
  const int width  = (*in_width), height = (*in_height); // 画像サイズ
  const int column = (gtype[0] >> 8) & 0x0F;
  const int w_table[5][5] = {{4, 5, 3, 5, 4}, {5, 2, 1, 2, 5}, {3, 1, 0, 1, 3},
    {5, 2, 1, 2, 5}, {4, 5, 3, 5, 4}};

  const double alpha[] = {-4.0, -2.0, -1.0, -0.5, 0.0, 0.5, 1.0, 2.0, 4.0},
        thr[]   = {-8.0, -4.0, -2.0, -1.0, -0.5, 0.0, 0.5, 1.0, 2.0, 4.0, 8.0},
        w[]     = {-8.0, -4.0, -2.0, -1.0, -0.5, 0.0, 0.5, 1.0, 2.0, 4.0, 8.0};

  int i, j, spd, position;
  int r_func[MAX_UNIT_SIZE], r_spd[MAX_UNIT_SIZE];
  double r_alpha[MAX_UNIT_SIZE], rw_thr[MAX_UNIT_SIZE];
  double rw_internal[INTERNAL_SIZE], rw_ext_input, rw_unit[MAX_UNIT_SIZE];

  for(i = 0; i < column + 2; i++) {
    position    = (column + 1 - i) * 4;
    r_func[i]   = (gtype[1] >> position) & 0x0F;
    r_alpha[i]  = alpha[(gtype[2] >> position) & 0x0F];
    r_spd[i]    = (gtype[3] >> position) & 0x0F;
    rw_thr[i]   = thr[(gtype[4] >> position) & 0x0F];
  }

  int ix, iy, posX, posY;
  double input_x, tmp_op;
  double internal_input[25], external_input, unit[MAX_UNIT_SIZE];
  double tmp_op_rfcn[MAX_UNIT_SIZE], op_rfcn[MAX_UNIT_SIZE];

  for(spd = 0; spd < 3; spd++) {
    for(i = 0; i < column + 2; i++) {
      input_x = 0.0;
      position = (column + 1 - i) * 4;

      if(r_spd[i] <= spd) {
        // 近傍入力の変換
        for(j = 0; j < INTERNAL_SIZE; j++)
          rw_internal[j] = w[(gtype[5 + j] >> position) & 0x0F];
        for(iy = 0; iy < 5; iy++) {
          posY = y + iy - 2;
          for(ix = 0; ix < 5; ix++) {
            posX = x + ix - 2;
            if(posX > -1 && posY > -1 &&
                posX < (width - 1) && posY < (height - 1)) {
              internal_input[iy * 5 + ix] =
                rw_internal[w_table[iy][ix]] * in_output[posY * width + posX];
            }
            else internal_input[iy * 5 + ix] = 0.0;
          }
        }

        // 外部入力の変換
        rw_ext_input    = w[(gtype[5 + INTERNAL_SIZE] >> position) & 0x0F];
        external_input = rw_ext_input * ext_output[y * width + x];

        // 隠れ層ユニットの変換
        for(j = 0; j < column; j++)
          rw_unit[j] = w[(gtype[5 + INTERNAL_SIZE + 1 + j] >> position) & 0x0F];
        for(j = 0; j < column; j++) {
          unit[j] = op_rfcn[j] * rw_unit[j];
        }
        if(r_func[i] < SUM) {
          for(j = 0; j < 25; j++)
            input_x += internal_input[j];
          input_x += external_input;
          for(j = 0; j < column; j++)
            input_x += unit[j];
          switch(r_func[i]) {
            case SIGMOID:   tmp_op = calc_sigmoid(input_x, r_alpha[i]);   break;
            case LINEAR:    tmp_op = calc_linear(input_x, r_alpha[i]);    break;
            case PIECEWISE: tmp_op = calc_piecewise(input_x, r_alpha[i]); break;
            case THRESHOLD: tmp_op = calc_threshold(input_x, r_alpha[i]); break;
            case GAUSSIAN:  tmp_op = calc_gaussian(input_x, r_alpha[i]);  break;
            case STEP:      tmp_op = calc_step(input_x, r_alpha[i]);      break;
          }
        }
        else {
          switch(r_func[i]) {
            case SUM: tmp_op = calc_sum(column, internal_input,
                          external_input, unit); break;
            case MAX: tmp_op = calc_max(column, internal_input,
                          external_input, unit); break;
            case MIN: tmp_op = calc_min(column, internal_input,
                          external_input, unit); break;
            case AVE: tmp_op = calc_average(column, internal_input,
                          external_input, unit); break;
            case RANGE: tmp_op = calc_range(column, internal_input,
                            external_input, unit); break;
          }
        }
        tmp_op_rfcn[i] = tmp_op;
      }
    } // for(i)
    for(i = 0; i < column + 2; i++) {
      op_rfcn[i]     = tmp_op_rfcn[i];
      tmp_op_rfcn[i] = 0.0;
    }
  } // for(spd)
  in_output[y * width + x]  = op_rfcn[column];
  ext_output[y * width + x] = op_rfcn[column + 1];
}
