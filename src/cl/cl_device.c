#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include "../../include/opencl.h"

cl_uint getDevices(cl_platform_id platform) {
  cl_uint num_devices;
  if(clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices) != CL_SUCCESS) {
    return 0;
  }
  return num_devices;
}

cl_device_id *getDeviceIDs(cl_platform_id platform, cl_uint num_devices) {
  cl_int status;
  cl_device_id *devices;

  devices = (cl_device_id *)calloc(num_devices, sizeof(cl_device_id));

  status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, num_devices,
      devices, NULL);
  if(status != CL_SUCCESS) {
    fprintf(stderr, "clGetDeviceIDs: %d\n", status);
    free(devices); devices = NULL;
    return NULL;
  }

  return devices;
}
