#include <cstdlib>
#include <math.h>
#include <stdio.h>

double function(double x) {
    return 2 * sin(x) - pow(x,2)/10;
}

double GoldenSection(double xL, double xU, double func(double)) {
  double x1, x2, d, r = 0.61803;
  d = r * (xU - xL);
  x1 = xL + d;
  x2 = xU - d;

  auto fx1 = func(x1);
  auto fx2 = func(x2);

  while (fabs(x1 - x2) > 3e-8) {
    if (func(x1) > func(x2)) {
      xL = x2;
      x2 = x1;

      d = r * (xU - xL);
      x1 = xL + d;

      fx1 = func(x1);
    } else {
      xU = x1;
      x1 = x2;

      d = r * (xU - xL);
      x2 = xU - d;
      fx2 = func(x2);
    }
  }
  return x1;
}

int main(void) {
  printf("%lf", GoldenSection(0, 3, &function));
  return 0;
}
