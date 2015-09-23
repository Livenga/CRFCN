#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <png.h>
#include "../../include/graphic.h"

void conv_weight(graphic teach, graphic input, graphic *weight) {
  int i, j;

  weight->width  = teach.width;
  weight->height = teach.height;
  weight->colors = teach.colors;

  weight->data = (uchar **)calloc(teach.height, sizeof(uchar *)); 
  for(i = 0; i < teach.height; i++) {
    weight->data[i] = (uchar *)calloc(teach.width * (teach.colors / 8), sizeof(uchar));

    for(j = 0; j < teach.width; j++) {
      weight->data[i][j] = 0xFF - input.data[i][j];
      //weight->data[i][j] = teach.data[i][j] - input.data[i][j];
      //kweight->data[i][j] = 0xFF - teach.data[i][j];
    }
  }
}

int conv_regulation(const int width, const int height, double *output) {
  int i, j, pos, do_nan = 0;

  for(i = 0; i < height; i++) {
    for(j = 0; j < width; j++) {
      pos = i * width + j;
      if(isnan(output[pos])) {
        output[pos] = 0.0;
        do_nan = 1;
      }
      if(output[pos] > 1.0) output[pos]      = 1.0;
      else if(output[pos] < 0.0) output[pos] = 0.0;
    }
  }
  return do_nan;
}
