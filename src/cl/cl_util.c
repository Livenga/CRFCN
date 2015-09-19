#include <stdlib.h>
#include <CL/cl.h>
#include "../../include/opencl.h"

void destroy_cl_prop(cl_prop *prop) {
  int i;

  // Kernel Code とパス領域の解放
  for(i = 0; i < prop->kcode.num_codes; i++) {
    free(prop->kcode.paths[i]); prop->kcode.paths[i] = NULL;
    free(prop->kcode.codes[i]); prop->kcode.codes[i] = NULL;
  }
  free(prop->kcode.paths); prop->kcode.paths = NULL;
  free(prop->kcode.codes); prop->kcode.codes = NULL;

  // OpenCL Platform 領域の解放
  free(prop->platforms); prop->platforms = NULL;

  // OpenCL Device 領域の解放
  free(prop->devices); prop->devices = NULL;

  clReleaseKernel(prop->kernel);
  clReleaseProgram(prop->program);
  clReleaseCommandQueue(prop->queue);
  clReleaseContext(prop->context);
}
