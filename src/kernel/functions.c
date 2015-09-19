double calc_sigmoid(double x, double a) {
  return 1.0 / (1.0 + exp(-a * x));
}

double calc_linear(double x, double a) {
  return a * x;
}

double calc_piecewise(double x, double a) {
  double tmp = a * x;
  if(tmp < 0.0) return 0.0;
  else if(0.0 < tmp && tmp <= 1.0) return tmp;
  else return 1.0;
  return 1.0;
}

double calc_threshold(double x, double a) {
  if(x > 0.0) return a * x;
  else return 0.0;
  return 0.0;
}

double calc_gaussian(double x, double a) {
  double left, right;
  left = 1.0 / sqrt(2.0 * M_PI * a * a);
  right = exp((-x * x) / (2.0 * a * a));
  return left * right;
}

double calc_step(double x, double a) {
  if(x > a) return 1.0;
  else if(x <= a) return 0.0;
  return 0.0;
}

double calc_sum(int column,
    double *internal_input, double external_input, double *hidden) {
  int i;
  double sum = 0.0;

  for(i = 0; i < 25; i++)
    sum += internal_input[i];
  sum += external_input;
  for(i = 0; i < column; i++)
    sum += hidden[i];
  return sum;
}

double calc_max(int column,
    double *internal_input, double external_input, double *hidden) {
  int i;
  double maxof = internal_input[0];

  for(i = 1; i < 25; i++)
    if(maxof < internal_input[i]) maxof = internal_input[i];
  if(maxof < external_input) maxof = external_input;
  for(i = 0; i < column; i++)
    if(maxof < hidden[i]) maxof = hidden[i];
  return maxof;
}

double calc_min(int column,
    double *internal_input, double external_input, double *hidden) {
  int i;
  double minof = internal_input[0];

  for(i = 1; i < 25; i++)
    if(minof > internal_input[i]) minof = internal_input[i];
  if(minof > external_input) minof = external_input;
  for(i = 0; i < column; i++)
    if(minof > hidden[i]) minof = hidden[i];
  return minof;
}
