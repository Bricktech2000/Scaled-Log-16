#include "sl16.h"
#include <float.h>
#include <math.h>
#include <stdio.h>
#define M_E exp(1.0)
#define M_PI 4.0 * atan(1.0)
#define M_TAU 8.0 * atan(1.0)

int main(void) {
#define COMPARE(DBL, SL16) printf("%12f\t%12s\n", DBL, sl16_fmt(SL16))
  COMPARE(M_PI * pow(7.0, 2.0), sl16_mul(SL16_PI, sl16_square(sl16_from(7))));
  COMPARE(2.0 * sqrt(M_E), sl16_mul(SL16_TWO, sl16_sqrt(SL16_E)));
  COMPARE(-1.0, sl16_neg(SL16_ONE));
  COMPARE(0.0, sl16_from(0));
  COMPARE(DBL_MIN, SL16_MIN);
  COMPARE(-DBL_MIN, sl16_neg(SL16_MIN));
  COMPARE(log2(DBL_MAX), sl16_log2(SL16_MAX));
  printf("\n");
  COMPARE(1.0 + 2.0, sl16_add(SL16_ONE, SL16_TWO));
  COMPARE(1.0 + 1.0, sl16_add(SL16_ONE, SL16_ONE));
  COMPARE(1.0 + 0.5, sl16_add(SL16_ONE, SL16_HALF));
  COMPARE(1.0 + 0.0, sl16_add(SL16_ONE, SL16_MIN));
  COMPARE(1.0 - 0.5, sl16_sub(SL16_ONE, SL16_HALF));
  COMPARE(1.0 - 1.0, sl16_sub(SL16_ONE, SL16_ONE));
  COMPARE(1.0 - 2.0, sl16_sub(SL16_ONE, SL16_TWO));
  COMPARE(1.0 - 3.0, sl16_sub(SL16_ONE, SL16_THREE));
  COMPARE(0.0 + 2.0, sl16_add(SL16_MIN, SL16_TWO));
  printf("\n");
  COMPARE(0.5 * 3.0, sl16_mul(SL16_HALF, SL16_THREE));
  COMPARE(-0.5 * 3.0, sl16_mul(sl16_neg(SL16_HALF), SL16_THREE));
  COMPARE(0.5 * -3.0, sl16_mul(SL16_HALF, sl16_neg(SL16_THREE)));
  COMPARE(-0.5 * -3.0, sl16_mul(sl16_neg(SL16_HALF), sl16_neg(SL16_THREE)));
  COMPARE(1.0 / 3.0, sl16_div(SL16_ONE, SL16_THREE));
  COMPARE(-1.0 / 3.0, sl16_div(sl16_neg(SL16_ONE), SL16_THREE));
  COMPARE(1.0 / -3.0, sl16_div(SL16_ONE, sl16_neg(SL16_THREE)));
  COMPARE(-1.0 / -3.0, sl16_div(sl16_neg(SL16_ONE), sl16_neg(SL16_THREE)));
  COMPARE(22.0 / 7.0, sl16_div(sl16_from(22), sl16_from(7)));
  printf("\n");
  COMPARE(log2(5236.0), sl16_log2(sl16_from(5236)));
  COMPARE(exp2(8.0), sl16_exp2(sl16_from(8)));
  COMPARE(exp2(M_TAU), sl16_exp2(SL16_TAU));
  COMPARE(exp2(-3.0), sl16_exp2(sl16_neg(SL16_THREE)));
  COMPARE(exp2(0.5), sl16_exp2(SL16_HALF));
  COMPARE(exp2(0.0), sl16_exp2(SL16_MIN));
  COMPARE(log(923), sl16_log(sl16_from(923)));
  COMPARE(exp(5.0), sl16_exp(sl16_from(5)));
  printf("\n");
  COMPARE(pow(0.5, 1.0), sl16_powi(SL16_HALF, 1));
  COMPARE(pow(0.5, 2.0), sl16_powi(SL16_HALF, 2));
  COMPARE(pow(0.5, 3.0), sl16_powi(SL16_HALF, 3));
  COMPARE(pow(-3.0, 1.0), sl16_powi(sl16_neg(SL16_THREE), 1));
  COMPARE(pow(-3.0, 2.0), sl16_powi(sl16_neg(SL16_THREE), 2));
  COMPARE(pow(-3.0, 3.0), sl16_powi(sl16_neg(SL16_THREE), 3));
  COMPARE(pow(0.5, 1.0), sl16_pow(SL16_HALF, SL16_ONE));
  COMPARE(pow(0.5, 2.0), sl16_pow(SL16_HALF, SL16_TWO));
  COMPARE(pow(0.5, 3.0), sl16_pow(SL16_HALF, SL16_THREE));
  COMPARE(pow(-3.0, 1.0), sl16_pow(sl16_neg(SL16_THREE), SL16_ONE));
  COMPARE(pow(-3.0, 2.0), sl16_pow(sl16_neg(SL16_THREE), SL16_TWO));
  COMPARE(pow(-3.0, 3.0), sl16_pow(sl16_neg(SL16_THREE), SL16_THREE));
  COMPARE(pow(3.0, M_E), sl16_pow(SL16_THREE, SL16_E));
  printf("\n");
  COMPARE(pow(0.333333, 1.0 / 1.0), sl16_rooti(SL16_THIRD, 1));
  COMPARE(pow(0.333333, 1.0 / 2.0), sl16_rooti(SL16_THIRD, 2));
  COMPARE(pow(0.333333, 1.0 / 3.0), sl16_rooti(SL16_THIRD, 3));
  COMPARE(pow(2.0, 1.0 / 1.0), sl16_rooti(SL16_TWO, 1));
  COMPARE(pow(2.0, 1.0 / 2.0), sl16_rooti(SL16_TWO, 2));
  COMPARE(pow(2.0, 1.0 / 3.0), sl16_rooti(SL16_TWO, 3));
  COMPARE(pow(0.333333, 1.0 / 1.0), sl16_root(SL16_THIRD, SL16_ONE));
  COMPARE(pow(0.333333, 1.0 / 2.0), sl16_root(SL16_THIRD, SL16_TWO));
  COMPARE(pow(0.333333, 1.0 / 3.0), sl16_root(SL16_THIRD, SL16_THREE));
  COMPARE(pow(2.0, 1.0 / 1.0), sl16_root(SL16_TWO, SL16_ONE));
  COMPARE(pow(2.0, 1.0 / 2.0), sl16_root(SL16_TWO, SL16_TWO));
  COMPARE(pow(2.0, 1.0 / 3.0), sl16_root(SL16_TWO, SL16_THREE));
  printf("\n");
  COMPARE(-2.0 * -2.0, sl16_square(sl16_neg(SL16_TWO)));
  COMPARE(-2.0 * -2.0 * -2.0, sl16_cube(sl16_neg(SL16_TWO)));
  COMPARE(sqrt(0.5), sl16_sqrt(SL16_HALF));
  COMPARE(cbrt(0.5), sl16_cbrt(SL16_HALF));
  COMPARE(fmin(0.5, 1.0), sl16_min(SL16_HALF, SL16_ONE));
  COMPARE(fmin(-0.5, 1.0), sl16_min(sl16_neg(SL16_HALF), SL16_ONE));
  COMPARE(fmin(0.5, -1.0), sl16_min(SL16_HALF, sl16_neg(SL16_ONE)));
  COMPARE(fmin(-0.5, -1.0), sl16_min(sl16_neg(SL16_HALF), sl16_neg(SL16_ONE)));
  COMPARE(hypot(3.0, 4.0), sl16_hypot(sl16_from(3), sl16_from(4)));
#undef COMPARE
}
