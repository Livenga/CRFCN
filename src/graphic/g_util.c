#include <stdlib.h>
#include "../../include/graphic.h"

double *img_normalize(graphic gdata) {
  int i, j;
  double *output;

  output = (double *)calloc(gdata.width * gdata.height, sizeof(double));

  for(i = 0; i < gdata.height; i++)
    for(j = 0; j < gdata.width; j++)
      output[i * gdata.width + j] = (double)gdata.data[i][j] / 255.0;

  return output;

}

void destroy_graphic(graphic *gdata) {
  int i;
  for(i = 0; i < gdata->height; i++)
    free(gdata->data[i]);
  free(gdata->data);
}
