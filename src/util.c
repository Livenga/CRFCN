#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include "../include/opencl.h"

char **read_kernel_code(cl_prop prop) {
  int i;
  FILE *kernel_fp;
  char **kernel_buf;
  long kernel_len;

  kernel_buf = (char **)calloc(prop.kcode.num_codes, sizeof(char));
  for(i = 0; i < prop.kcode.num_codes; i++) {
    if((kernel_fp = fopen(prop.kcode.paths[i], "r")) == NULL) {
      perror(prop.kcode.paths[i]); return NULL;
    }

    fseek(kernel_fp, 0L, SEEK_END);
    kernel_len = ftell(kernel_fp);
    fseek(kernel_fp, 0L, SEEK_SET);

    kernel_buf[i] = (char *)calloc(kernel_len, sizeof(char));

    fread(kernel_buf[i], sizeof(char), kernel_len, kernel_fp);
    fclose(kernel_fp);
  }

  return kernel_buf;
}

char **code_paths(int *cnt, const char *dir) {
  DIR    *o_dir;
  struct dirent *r_dir;
  char *p, **paths;
  size_t dir_length = strlen(dir), path_length;
  paths = (char **)calloc(1, sizeof(char *));
  if((o_dir = opendir(dir)) == NULL) {perror(dir); return NULL;}
  while((r_dir = readdir(o_dir)) != NULL) {
    if((p = strstr(r_dir->d_name, ".c")) != NULL) {
      path_length = strlen(r_dir->d_name);

      paths[(*cnt)] = (char *)calloc(dir_length + path_length, sizeof(char));
      sprintf(paths[(*cnt)], "%s/%s", dir, r_dir->d_name);

      (*cnt)++;
      paths = (char **)realloc(paths, sizeof(char *) * (*cnt));
    }
  }
  closedir(o_dir);
  return paths;
}
