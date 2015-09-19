#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include "../../include/opencl.h"

int setProperties(const char *binary, int pid, int did, cl_prop *prop) {
  extern int f_binary;
  cl_int status;
  cl_context_properties properties[] = {
    CL_CONTEXT_PLATFORM, (cl_context_properties)prop->platforms[pid], 0};

  prop->context = clCreateContext(properties, prop->num_devices,
      (const cl_device_id *)prop->devices, NULL, NULL, &status);
  if(status != CL_SUCCESS) {
    fprintf(stderr, "clCreateContext: %d\n", status); return EOF;
  }

  cl_queue_properties queue_properties[] = {
    CL_QUEUE_PROPERTIES,
    //CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE | CL_QUEUE_ON_DEVICE | CL_QUEUE_ON_DEVICE_DEFAULT,
    CL_QUEUE_PROFILING_ENABLE,
    //CL_QUEUE_SIZE,
    0
  };

  prop->queue = clCreateCommandQueueWithProperties(prop->context,
      prop->devices[did], (const cl_queue_properties *)queue_properties, &status);
  if(status != CL_SUCCESS) {
    fprintf(stderr, "clCreateCommandQueueWithProperties: %d\n", status); return EOF;
  }

  prop->program = clCreateProgramWithSource(prop->context,
      prop->kcode.num_codes, (const char **)prop->kcode.codes, NULL, &status);
  if(status != CL_SUCCESS) {
    fprintf(stderr, "clCreateProgramWithSource: %d\n", status); return EOF;
  }

  status = clBuildProgram(prop->program, prop->num_devices,
      (const cl_device_id *)prop->devices, NULL, NULL, NULL);

  //if(status != CL_SUCCESS) {
    char buffer[4096];
    clGetProgramBuildInfo(prop->program, prop->devices[did], CL_PROGRAM_BUILD_LOG,
        4096 * sizeof(char), buffer, NULL);
    printf("\033[1m[Build Log]\033[0m\n");
    printf("\033[1m%s\033[0m\n\n", buffer);
  //}

  if(f_binary) {
    FILE *binary_fp;
    size_t binary_size;
    unsigned char *binary_char;

    if((binary_fp = fopen(binary, "w")) != NULL) {
      clGetProgramInfo(prop->program, CL_PROGRAM_BINARY_SIZES, sizeof(size_t), &binary_size, NULL);

      binary_char = (unsigned char *)calloc(binary_size, sizeof(unsigned char));
      clGetProgramInfo(prop->program, CL_PROGRAM_BINARIES, sizeof(unsigned char *) * binary_size,
          (unsigned char **)&binary_char, NULL);

      fwrite(binary_char, sizeof(unsigned char), binary_size, binary_fp);
      free(binary_char); binary_char = NULL;
      fclose(binary_fp);
    }
  }

  prop->kernel = clCreateKernel(prop->program, (const char *)"rfcn", &status);

  return 0;
}
