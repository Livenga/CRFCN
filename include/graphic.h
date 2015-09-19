#ifndef GRAPHIC_H
#define GRAPHIC_H

typedef unsigned char uchar;
typedef struct _graphic {
  int width, height, colors;
  uchar **data;
} graphic;

#ifndef PNG_H
#include <png.h>
#endif
/* src/graphic/g_png.c */
extern int pnread(const char *path, graphic *gdata);
extern int pnwrite(const char *path, const graphic gdata);
extern int pnwrite_from_double(const char *path, const int width, const int height, double *output);

/* src/graphic/g_util.c */
extern double *img_normalize(graphic gdata);
extern void destroy_graphic(graphic *gdata);

/* src/graphic/g_conv.c */
extern void conv_weight(graphic teach, graphic input, graphic *weight);
#endif
