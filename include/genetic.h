#define HEADER_SIZE       (1)
#define PARAMETER_SIZE    (4)
#define INTERNAL_SIZE     (6)
#define EXTERNAL_INPUT    (1)
#define EXTERNAL_OUTPUT   (1)
#define INTERNAL_OUTPUT   (1)
#define DEFAULT_UNIT_SIZE (4)
#define MAX_UNIT_SIZE     (8)
#define EXTENDED_SIZE     (MAX_UNIT_SIZE - DEFAULT_UNIT_SIZE)

#define DEFAULT_GENOTYPE_SIZE (HEADER_SIZE + PARAMETER_SIZE + \
    INTERNAL_SIZE + EXTERNAL_INPUT + DEFAULT_UNIT_SIZE + EXTERNAL_OUTPUT)
#define MAX_GENOTYPE_SIZE (DEFAULT_GENOTYPE_SIZE +\
    EXTENDED_SIZE)

typedef unsigned int genotype_t;
typedef genotype_t * genotype;

#define CRFCN_VER_STRING "1.0.0"

#ifndef KERNEL_FUNC
#define NUMBER_OF_GENERATION (20000)
#define POPULATION_SIZE      (50)
//#define POPULATION_SIZE      (1)
#define CHILDREN_SIZE        (30)

/* src/genetic/gn_init.c */
extern genotype galloc(void);
extern void ginit(genotype gtype);

/* src/genetic/gn_util.c */
extern void free_genotype(genotype *gtype, const int population);

/* src/genetic/gn_crossover.c */
extern void gcrossover(genotype *ch_gtype, genotype pr_gtype1, genotype pr_gtype2,
    const int population);

/* src/genetic/gn_selection.c */
extern int numof_best_fitness(const double *fitness, const int population);
extern int numof_roulette(const double *fitness, const int population);

/* src/genetic/gn_print.c */
extern void gprint(const int num, genotype gtype);
/* src/calc/fitness.c */
extern double calc_fitness(const int width, const int height,
    double *t, double *o, double *w);

enum function_number {
  SIGMOID, LINEAR, PIECEWISE, THRESHOLD, GAUSSIAN, STEP, SUM, MAX, MIN,
  MED, AVE, RANGE
};
#else

#define SIGMOID   (0)
#define LINEAR    (1)
#define PIECEWISE (2)
#define THRESHOLD (3)
#define GAUSSIAN  (4)
#define STEP      (5)
#define SUM       (6)
#define MAX       (7)
#define MIN       (8)
#define MED       (9)
#define AVE       (10)
#define RANGE     (11)

double calc_sigmoid(double x, double a);
double calc_linear(double x, double a);
double calc_piecewise(double x, double a);
double calc_threshold(double x, double a);
double calc_gaussian(double x, double a);
double calc_step(double x, double a);
#endif
