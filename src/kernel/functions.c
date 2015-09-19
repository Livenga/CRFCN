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
