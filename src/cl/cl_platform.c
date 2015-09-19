#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

cl_uint getPlatforms(void) {
  cl_int status;
  cl_uint num_platforms;
  if((status = clGetPlatformIDs(0, NULL, &num_platforms)) != CL_SUCCESS) {
    fprintf(stderr, "clGetPlatformIDs: %d\n", status);
    return 0;
  }
  return num_platforms;
}

cl_platform_id *getPlatformIDs(cl_uint num_platforms) {
  cl_int status;
  cl_platform_id *platforms;

  platforms = (cl_platform_id *)calloc(num_platforms, sizeof(cl_platform_id));
  status = clGetPlatformIDs(num_platforms, platforms, NULL);
  if(status != CL_SUCCESS) {
    fprintf(stderr, "clGetPlatformIDs: %d\n", status);
    free(platforms); platforms = NULL;
    return NULL;
  }

  return platforms;
}
