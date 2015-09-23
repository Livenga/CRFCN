#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <png.h>
#include "../../include/graphic.h"

int pnread(const char *path, graphic *gdata) {
  FILE *pfp;
  png_structp png_ptr;
  png_infop info_ptr;

  if((pfp = fopen(path, "rb")) == NULL) {perror(path); return EOF;}

  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  info_ptr = png_create_info_struct(png_ptr);

  png_init_io(png_ptr, pfp);

  png_read_info(png_ptr, info_ptr);

  png_uint_32 pwidth, pheight;
  png_byte colors, bit_depth;

  pwidth     = png_get_image_width(png_ptr, info_ptr);
  pheight    = png_get_image_height(png_ptr, info_ptr);
  colors     = png_get_color_type(png_ptr, info_ptr);
  bit_depth = png_get_bit_depth(png_ptr, info_ptr);

  switch(colors) {
    case PNG_COLOR_TYPE_GRAY: colors = bit_depth * 1; break;
    case PNG_COLOR_TYPE_GA:   colors = bit_depth * 2; break;
    case PNG_COLOR_TYPE_RGB:  colors = bit_depth * 3; break;
    case PNG_COLOR_TYPE_RGBA: colors = bit_depth * 4; break;
  }
  gdata->width  = pwidth; gdata->height = pheight;
  gdata->colors = colors;
  colors /= 8;

  int i;
  gdata->data = (uchar **)calloc(pheight, sizeof(uchar *));
  for(i = 0; i < pheight; i++)
    gdata->data[i] = (uchar *)calloc(pwidth * colors, sizeof(uchar));

  png_read_image(png_ptr, (png_bytepp)gdata->data);
  png_read_end(png_ptr, info_ptr);

  png_destroy_info_struct(png_ptr, &info_ptr);
  png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
  fclose(pfp);
  return 0;
}

int pnwrite(const char *path, const graphic gdata) {
  FILE *pfp;
  png_structp png_ptr;
  png_infop info_ptr;

  if((pfp = fopen(path, "wb")) == NULL) {perror(path); return EOF;}
  png_ptr  = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  info_ptr = png_create_info_struct(png_ptr);

  png_init_io(png_ptr, pfp);

  png_set_IHDR(png_ptr, info_ptr,
      gdata.width, gdata.height, 8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
      PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

  png_write_info(png_ptr, info_ptr);
  png_write_image(png_ptr, (png_bytepp)gdata.data);
  png_write_end(png_ptr, info_ptr);

  png_destroy_info_struct(png_ptr, &info_ptr);
  png_destroy_write_struct(&png_ptr, &info_ptr);

  fclose(pfp);
  return 0;
}

int pnwrite_from_double(const char *path,
    const int width, const int height, double *output) {
  FILE *pfp;
  png_structp png_ptr;
  png_infop info_ptr;
  uchar **data;

  if((pfp = fopen(path, "wb")) == NULL) {perror(path); return EOF;}
  png_ptr  = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  info_ptr = png_create_info_struct(png_ptr);

  png_init_io(png_ptr, pfp);

  png_set_IHDR(png_ptr, info_ptr,
      width, height, 8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
      PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

  int i, j;
  data = (uchar **)calloc(height, sizeof(uchar *));
  for(i = 0; i < height; i++) {
    data[i] = (uchar *)calloc(width, sizeof(uchar));
    for(j = 0; j < width; j++) {
      if(isnan(output[i * width + j]))     data[i][j] = 1.0;
      if(output[i * width + j] > 1.0)      data[i][j] = 1.0;
      else if(output[i * width + j] < 0.0) data[i][j] = 0.0;
      data[i][j] = output[i * width + j] * 255.0;
    }
  }

  png_write_info(png_ptr, info_ptr);
  png_write_image(png_ptr, (png_bytepp)data);
  png_write_end(png_ptr, info_ptr);

  png_destroy_info_struct(png_ptr, &info_ptr);
  png_destroy_write_struct(&png_ptr, &info_ptr);

  for(i = 0; i < height; i++) {
    free(data[i]); data[i] = NULL;
  }
  free(data); data = NULL;
  fclose(pfp);
  return 0;
}
