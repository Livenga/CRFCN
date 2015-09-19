#ifndef CRFCN_CL_H
#define CRFCN_CL_H
//#define DEBUG

#ifndef __OPENCL_H
#include <CL/cl.h>
#endif /* __OPENCL_H */

typedef struct _cl_prop {
  struct kcode {
    int num_codes;
    char **paths;
    char **codes;
  } kcode;

  cl_uint num_platforms;
  cl_platform_id *platforms;

  cl_uint num_devices;
  cl_device_id *devices;

  cl_context context;
  cl_command_queue queue;

  cl_program program;
  cl_kernel kernel;
} cl_prop;

/* src/util.c */
extern char **read_kernel_code(cl_prop prop);
extern char **code_paths(int *cnt, const char *dir);

/* src/cl/cl_platform.c */
extern cl_uint getPlatforms(void);
extern cl_platform_id *getPlatformIDs(cl_uint num_platforms);

/* src/cl/cl_device.c */
extern cl_uint getDevices(cl_platform_id platform);
extern cl_device_id *getDeviceIDs(cl_platform_id platform, cl_uint num_devices);

/* src/cl/cl_util.c */
extern void destroy_cl_prop(cl_prop *prop);

/* src/cl/cl_prop.c */
extern int setProperties(const char *binary, int pid, int did, cl_prop *prop);

/* src/cl/cl_print.c */
extern void print_platform(cl_prop prop);
extern void print_device(cl_prop prop);
/* src/cl/cl_error.c */
extern void cl_print_error(const char *str_func, const cl_int status);
#endif /* CRFCN_CL_H */
