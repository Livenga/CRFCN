#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include "../../include/opencl.h"

const char *device_type(const cl_device_type type);
void print_platform(cl_prop prop) {
  int i;
  char buffer[1024];

  for(i = 0; i < prop.num_platforms; i++) {
    printf("Platform No.%2d\n", i);

    clGetPlatformInfo(prop.platforms[i], CL_PLATFORM_VERSION, 1024, buffer, NULL);
    printf("[Platform Version]    %s\n", buffer);

    clGetPlatformInfo(prop.platforms[i], CL_PLATFORM_NAME, 1024, buffer, NULL);
    printf("[Platform Name]       %s\n", buffer);

    clGetPlatformInfo(prop.platforms[i], CL_PLATFORM_VENDOR, 1024, buffer, NULL);
    printf("[Platform Vendor]     %s\n", buffer);

    clGetPlatformInfo(prop.platforms[i], CL_PLATFORM_EXTENSIONS, 1024, buffer, NULL);
    printf("[Platform Extensions] %s\n", buffer);
    putchar('\n');
  }
}

void print_device(cl_prop prop) {
  int i, j;
  char buffer[1024];
  cl_bool isenable;
  cl_device_type type;
  cl_uint uint_val;
  size_t *item_size;


  for(i = 0; i < prop.num_devices; i++) {
    printf("Device No.%2d\n", i);

    clGetDeviceInfo(prop.devices[i], CL_DEVICE_NAME, 1024, buffer, NULL);
    printf("[Device Name]                 %s\n", buffer);

    clGetDeviceInfo(prop.devices[i], CL_DEVICE_VENDOR, 1024, buffer, NULL);
    printf("[Device Vendor]               %s\n", buffer);

    clGetDeviceInfo(prop.devices[i], CL_DEVICE_VENDOR_ID, sizeof(cl_uint), &uint_val, NULL);
    printf("[Device Vendor ID]            %u\n", uint_val);

    clGetDeviceInfo(prop.devices[i], CL_DEVICE_AVAILABLE, sizeof(cl_bool), &isenable, NULL);
    printf("[Device Available]            %s\n", (isenable == CL_TRUE) ? "True" : "False");

    clGetDeviceInfo(prop.devices[i], CL_DEVICE_TYPE, sizeof(cl_device_type), &type, NULL);
    printf("[Device Type]                 %s\n", device_type(type));

    clGetDeviceInfo(prop.devices[i], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(cl_uint), &uint_val, NULL);
    printf("[Device Max Frequency]        %u[MHz]\n", uint_val);

    clGetDeviceInfo(prop.devices[i], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &uint_val, NULL);
    printf("[Device Max Compute Units]    %u\n", uint_val);

    clGetDeviceInfo(prop.devices[i], CL_DEVICE_PROFILE, 1024, buffer, NULL);
    printf("[Device Profile]              %s\n", buffer);

    clGetDeviceInfo(prop.devices[i], CL_DEVICE_EXTENSIONS, 1024, buffer, NULL);
    printf("[Device Extensions]           %s\n", buffer);


    clGetDeviceInfo(prop.devices[i], CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), &uint_val, NULL);
    printf("[Device Work Item Dimensions] %u\n", uint_val);
    item_size = (size_t *)calloc(uint_val, sizeof(size_t));
    clGetDeviceInfo(prop.devices[i], CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size_t) * uint_val, item_size, NULL);
    for(j = 0; j < uint_val; j++)
      printf("[Device Work Item Size %2d]    %ld\n", j, item_size[j]);
    free(item_size);
  }
}

const char *device_type(const cl_device_type type) {
  const char *msg;
  switch(type) {
    case CL_DEVICE_TYPE_CPU: msg = "CPU"; break;
    case CL_DEVICE_TYPE_GPU: msg = "GPU"; break;
    case CL_DEVICE_TYPE_ACCELERATOR: msg = "Accelerator"; break;
    case CL_DEVICE_TYPE_DEFAULT: msg = "Default"; break;
  }
  return msg;
}
