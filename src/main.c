#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <getopt.h>
#include <CL/cl.h>
#include "../include/opencl.h"
#include "../include/graphic.h"
#include "../include/genetic.h"

extern void genetic_operation(cl_prop prop, graphic teach, graphic input, graphic weight);
void sig_handle(int sig);
int sig_quit;

struct option long_opt[] = {
  {"help",         no_argument, 0, 'h'},
  {"version",      no_argument, 0, 'v'},
  {"binary", required_argument, 0, 'b'},
  {0, 0, 0, 0},
};
int f_help, f_ver, f_binary;

void print_help(const char *msg);
void print_version(const char *msg);

int main(int argc, char *argv[])
{
  const int pid = 0, did = 0;
  int opt, optindex;
  char *p;
  graphic teach_data, in_data, weight_data;

  cl_prop prop;
  memset(&prop, '\0', sizeof(cl_prop));
  signal(SIGINT, sig_handle);

  char binary_path[1024];
  while((opt = getopt_long(argc, argv, "hvbia:",
          long_opt, &optindex)) != EOF) {
    switch(opt) {
      case 'h': f_help = 1; break;
      case 'v': f_ver = 1;  break;
      case 'b': f_binary = 1;
                p = (optarg != NULL) ? optarg : argv[optind];
                if(p == NULL) goto option_error;
                strcpy(binary_path, p);
                break;
    }
  }
  if(f_help) print_help(argv[0]);
  if(f_ver) print_version(argv[0]);

  /* OpenCL設定 */
  prop.kcode.paths   = code_paths(&prop.kcode.num_codes, "src/kernel");
  prop.kcode.codes   = read_kernel_code(prop);

  prop.num_platforms = getPlatforms();
  prop.platforms     = getPlatformIDs(prop.num_platforms);
  prop.num_devices   = getDevices(prop.platforms[pid]);
  prop.devices       = getDeviceIDs(prop.platforms[pid], prop.num_devices);

  setProperties(binary_path, pid, did, &prop);

  /* 画像 */
  pnread("images/text_mono_50.png", &teach_data);
  pnread("images/gleft_50.png", &in_data);

#ifdef DEBUG
  printf("\033[1m\033[36m[OpenCL]\033[0m\033[39m\n");
  printf("  [Number of Platforms] %d\n", prop.num_platforms);
  printf("  [Number of Devices]   %d\n\n", prop.num_devices);

  print_platform(prop);
  print_device(prop);

  putchar('\n');
#endif
  printf("\033[1m\033[36m[Graphic Information]\033[0m\033[39m\n");
  printf("  [Image Size]       %dx%d\n", teach_data.width, teach_data.height);
  printf("  [Image ColorSpace] %d\n", teach_data.colors / 8);

  conv_weight(teach_data, in_data, &weight_data);
  //pnwrite("weight_image.png", weight_data);

  genetic_operation(prop, teach_data, in_data, weight_data);

  // 破棄
  destroy_graphic(&weight_data);
  destroy_graphic(&in_data);
  destroy_graphic(&teach_data);
  destroy_cl_prop(&prop);
  return 0;

option_error:
  fprintf(stderr, "オプションにエラーが存在します.\n");
  return EOF;
}

void print_help(const char *msg) {
  printf("\033[1m使用法: %s [オプション]... [ファイル]...\n\033[0m", msg);
  printf("[オプション]\n");
  printf("  -h, --help: このヘルプ情報を表示します.\n");
  printf("  -b, --binary: OpenCL Kernelのバイナリファイルを出力.\n");
  putchar('\n');
  exit(0);
}

void print_version(const char *msg) {
  printf("\033[1m%s: Version %s Copyrignt 2015 Livenga. All right reserved.\033[0m\n", msg, CRFCN_VER_STRING);
  exit(0);
}

void sig_handle(int sig) {
  printf("[-] Forced Termination...\n");
  sig_quit = 1;

  signal(SIGINT, SIG_DFL);
}
