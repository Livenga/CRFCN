#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/opencl.h"
#include "../include/graphic.h"
#include "../include/genetic.h"

double alpha[] = {-4.0, -2.0, -1.0, -0.5, 0.0, 0.5, 1.0, 2.0, 4.0},
       thr[]   = {-8.0, -4.0, -2.0, -1.0, -0.5, 0.0, 0.5, 1.0, 2.0, 4.0, 8.0},
       w[]     = {-8.0, -4.0, -2.0, -1.0, -0.5, 0.0, 0.5, 1.0, 2.0, 4.0, 8.0};
int alpha_size = sizeof(alpha) / sizeof(alpha[0]),
    thr_size   = sizeof(thr)   / sizeof(thr[0]),
    w_size     = sizeof(w)     / sizeof(w[0]);

typedef short boolean;
extern void printd(const char *str);

void genetic_operation(cl_prop prop, graphic teach, graphic input, graphic weight) {
  const boolean __P_DEBUG = 1;
  const int img_size   = teach.width * teach.height;
  const int gtype_size = sizeof(genotype_t) * MAX_GENOTYPE_SIZE;
  extern int sig_quit;
  // 通常変数
  int i, tr, transition, cnt_generation = 0;
  size_t global_size[2];
  // 最適値
  double pr_fitness[POPULATION_SIZE], ch_fitness[CHILDREN_SIZE];
  double *n_teach, *n_input, *n_weight, *n_in_output, *n_ext_output;
  // 遺伝子型
  genotype pr_gtype[POPULATION_SIZE], ch_gtype[CHILDREN_SIZE];
  // OpenCL用変数
  cl_int status;
  cl_mem in_width, in_height, cl_gtype;
  cl_mem cl_input, in_output, ext_output;

  srand((unsigned)time(NULL)); // ランダム値のシードを生成

  // 画像データの正規化
  n_teach   = img_normalize(teach);
  n_input   = img_normalize(input);
  n_weight  = img_normalize(weight);
  n_in_output  = (double *)calloc(img_size, sizeof(double));
  n_ext_output = (double *)calloc(img_size, sizeof(double));

  // OpenCL Kernel関数 引数の設定
  in_width = clCreateBuffer(prop.context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
      sizeof(int), (void *)&teach.width, &status);
  in_height = clCreateBuffer(prop.context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
      sizeof(int), (void *)&teach.height, &status);
  cl_gtype = clCreateBuffer(prop.context, CL_MEM_READ_ONLY,
      gtype_size, NULL, &status);
  in_output = clCreateBuffer(prop.context, CL_MEM_READ_WRITE,
      sizeof(double) * img_size, NULL, NULL);
  ext_output = clCreateBuffer(prop.context, CL_MEM_READ_WRITE,
      sizeof(double) * img_size, NULL, NULL);
  // 次元サイズ
  //global_size[0] = 1; global_size[1] = 1;
  global_size[0] = teach.width; global_size[1] = teach.height;
  // 引数の設定
  clSetKernelArg(prop.kernel, 0, sizeof(cl_mem), (const void *)&in_width);
  clSetKernelArg(prop.kernel, 1, sizeof(cl_mem), (const void *)&in_height);
  clSetKernelArg(prop.kernel, 2, sizeof(cl_mem), (const void *)&cl_gtype);
  clSetKernelArg(prop.kernel, 3, sizeof(cl_mem), (const void *)&in_output);
  clSetKernelArg(prop.kernel, 4, sizeof(cl_mem), (const void *)&ext_output);

  char work_dir[1024];
  for(i = 0; i < POPULATION_SIZE; i++) {
    pr_gtype[i] = galloc();
    ginit(pr_gtype[i]);
    //gprint(i, pr_gtype[i]);

    memmove(n_in_output,  n_input, sizeof(double) * img_size);
    memmove(n_ext_output, n_input, sizeof(double) * img_size);

    transition = (pr_gtype[i][0] >> 16) & 0xFF;              // 遷移回数の取得
    clEnqueueWriteBuffer(prop.queue, cl_gtype, CL_TRUE, 0,   // 遺伝子型の書き込み
        gtype_size, (const void *)pr_gtype[i], 0, NULL, NULL);

    for(tr = 0; tr < transition; tr++) {                   // 遷移回数分回す
      // Kernel関数の実行
      clEnqueueWriteBuffer(prop.queue, in_output, CL_TRUE, 0,  // 内部入力
          sizeof(double) * img_size, (const void *)n_in_output, 0, NULL, NULL);
      clEnqueueWriteBuffer(prop.queue, ext_output, CL_TRUE, 0, // 外部入力
          sizeof(double) * img_size, (const void *)n_ext_output, 0, NULL, NULL);

      status = clEnqueueNDRangeKernel(prop.queue, prop.kernel, 2, NULL, global_size,
          NULL, 0, NULL, NULL);

      clEnqueueReadBuffer(prop.queue, in_output, CL_TRUE, 0,
          sizeof(double) * img_size, (void *)n_in_output, 0, NULL, NULL);
      clEnqueueReadBuffer(prop.queue, ext_output, CL_TRUE, 0,
          sizeof(double) * img_size, (void *)n_ext_output, 0, NULL, NULL);
    }
    conv_regulation(teach.width, teach.height, n_ext_output);
    sprintf(work_dir, "works/crfcn_in%03d.png", i);
    pnwrite_from_double(work_dir, teach.width, teach.height, n_in_output);
    sprintf(work_dir, "works/crfcn_ex%03d.png", i);
    pnwrite_from_double(work_dir, teach.width, teach.height, n_ext_output);

    pr_fitness[i] = calc_fitness(teach.width, teach.height,
        n_teach, n_ext_output, n_weight);
    printf("[個体集合 No.%3d] 適応度 %f\n", i, pr_fitness[i]);
  }
  if(__P_DEBUG) printd("[*] 個体領域の確保と初期化に成功.\n");

  for(i = 0; i < CHILDREN_SIZE; i++)
    ch_gtype[i] = galloc();
  if(__P_DEBUG) printd("[*] 子個体領域の確保に成功.\n");

  int slt_rand[2], slt_best, slt_roul, best_num;
  do {
    slt_rand[0] = rand() % POPULATION_SIZE;
    slt_rand[1] = rand() % POPULATION_SIZE;
    while(slt_rand[0] == slt_rand[1])
      slt_rand[1] = rand() % POPULATION_SIZE;

    memmove(ch_gtype[0], pr_gtype[slt_rand[0]],
        sizeof(genotype_t) * MAX_GENOTYPE_SIZE);
    memmove(ch_gtype[1], pr_gtype[slt_rand[1]],
        sizeof(genotype_t) * MAX_GENOTYPE_SIZE);
    // 交叉
    gcrossover(ch_gtype, pr_gtype[slt_rand[0]], pr_gtype[slt_rand[1]],
        CHILDREN_SIZE);

    for(i = 0; i < CHILDREN_SIZE; i++) {
      memmove(n_in_output,  n_input, sizeof(double) * img_size);
      memmove(n_ext_output, n_input, sizeof(double) * img_size);

      transition = (ch_gtype[i][0] >> 16) & 0xFF;              // 遷移回数の取得
      clEnqueueWriteBuffer(prop.queue, cl_gtype, CL_TRUE, 0,   // 遺伝子型の書き込み
          gtype_size, (const void *)ch_gtype[i], 0, NULL, NULL);

      for(tr = 0; tr < transition; tr++) {                   // 遷移回数分回す
        // Kernel関数の実行
        clEnqueueWriteBuffer(prop.queue, in_output, CL_TRUE, 0,  // 内部入力
            sizeof(double) * img_size, (const void *)n_in_output, 0, NULL, NULL);
        clEnqueueWriteBuffer(prop.queue, ext_output, CL_TRUE, 0, // 外部入力
            sizeof(double) * img_size, (const void *)n_ext_output, 0, NULL, NULL);

        status = clEnqueueNDRangeKernel(prop.queue, prop.kernel, 2, NULL, global_size,
            NULL, 0, NULL, NULL);

        clEnqueueReadBuffer(prop.queue, in_output, CL_TRUE, 0,
            sizeof(double) * img_size, (void *)n_in_output, 0, NULL, NULL);
        clEnqueueReadBuffer(prop.queue, ext_output, CL_TRUE, 0,
            sizeof(double) * img_size, (void *)n_ext_output, 0, NULL, NULL);
      }
      conv_regulation(teach.width, teach.height, n_ext_output);
      ch_fitness[i] = calc_fitness(teach.width, teach.height,
          n_teach, n_ext_output, n_weight);
    }

    slt_best = numof_best_fitness(ch_fitness, CHILDREN_SIZE);
    slt_roul = numof_roulette(ch_fitness, CHILDREN_SIZE);
    while(slt_best == slt_roul ||
        (slt_roul == (CHILDREN_SIZE + 1)))
      slt_roul = numof_roulette(ch_fitness, CHILDREN_SIZE);

    best_num = numof_best_fitness(pr_fitness, POPULATION_SIZE);
    if(cnt_generation % 2 == 0) {
      printf("[経過世代数] %5d\n", cnt_generation);
      printf("[個体集合ランダム選択]        %2d(%f) and %2d(%f)\n",
          slt_rand[0], pr_fitness[slt_rand[0]],
          slt_rand[1], pr_fitness[slt_rand[1]]);
      printf("[子個体集合 最高とルーレット] %2d(%f) and %2d(%f)\n\n",
          slt_best, ch_fitness[slt_best], slt_roul, ch_fitness[slt_roul]);
      printf("[個体集合最高適応度] %2d(%f)\n\n", best_num, pr_fitness[best_num]);
      gprint(best_num, pr_gtype[best_num]);
    }

    pr_fitness[slt_rand[0]] = ch_fitness[slt_best];
    pr_fitness[slt_rand[1]] = ch_fitness[slt_roul];

    memmove(pr_gtype[slt_rand[0]], ch_gtype[slt_best], sizeof(genotype_t) * MAX_GENOTYPE_SIZE);
    memmove(pr_gtype[slt_rand[1]], ch_gtype[slt_roul], sizeof(genotype_t) * MAX_GENOTYPE_SIZE);

    if(EPSILON < pr_fitness[best_num]) break;
    if(sig_quit) break;
  } while(cnt_generation++ != NUMBER_OF_GENERATION);

  printf("[最終世代] %d\n", cnt_generation);
  printf("最適値: %f(%d)\n", pr_fitness[best_num], best_num);

  best_num = numof_best_fitness(pr_fitness, POPULATION_SIZE);
  gprint(best_num, pr_gtype[best_num]);
  memmove(n_in_output,  n_input, sizeof(double) * img_size);
  memmove(n_ext_output, n_input, sizeof(double) * img_size);

  transition = (pr_gtype[best_num][0] >> 16) & 0xFF;              // 遷移回数の取得
  clEnqueueWriteBuffer(prop.queue, cl_gtype, CL_TRUE, 0,   // 遺伝子型の書き込み
      gtype_size, (const void *)pr_gtype[best_num], 0, NULL, NULL);

  char best_output[1024];
  for(tr = 0; tr < transition; tr++) {                   // 遷移回数分回す
    // Kernel関数の実行
    clEnqueueWriteBuffer(prop.queue, in_output, CL_TRUE, 0,  // 内部入力
        sizeof(double) * img_size, (const void *)n_in_output, 0, NULL, NULL);
    clEnqueueWriteBuffer(prop.queue, ext_output, CL_TRUE, 0, // 外部入力
        sizeof(double) * img_size, (const void *)n_ext_output, 0, NULL, NULL);

    status = clEnqueueNDRangeKernel(prop.queue, prop.kernel, 2, NULL, global_size,
        NULL, 0, NULL, NULL);

    clEnqueueReadBuffer(prop.queue, in_output, CL_TRUE, 0,
        sizeof(double) * img_size, (void *)n_in_output, 0, NULL, NULL);
    clEnqueueReadBuffer(prop.queue, ext_output, CL_TRUE, 0,
        sizeof(double) * img_size, (void *)n_ext_output, 0, NULL, NULL);

    sprintf(best_output, "best/in_output%02d.png", tr);
    pnwrite_from_double(best_output, teach.width, teach.height, n_in_output);
    sprintf(best_output, "best/ext_output%02d.png", tr);
    pnwrite_from_double(best_output, teach.width, teach.height, n_ext_output);
  }

db_point:
  clReleaseMemObject(in_width);
  clReleaseMemObject(in_height);
  clReleaseMemObject(cl_gtype);
  clReleaseMemObject(in_output);
  clReleaseMemObject(ext_output);

  free_genotype(pr_gtype, POPULATION_SIZE);
  free_genotype(ch_gtype, CHILDREN_SIZE);

  if(__P_DEBUG) printd("[*] 遺伝子型領域の解放に成功.\n");

  free(n_teach);  free(n_input); free(n_weight);
  free(n_in_output); free(n_ext_output);
  if(__P_DEBUG) printd("[*] 正規化画像データの解放に成功.\n");
}
