#include "sl16.h"
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// generalization of left shift that can shift past the bit with of the LHS and
// that will right-shift when given a negative RHS
#define SANE_SHIFT(N, S)                                                       \
  ((S) >= 0 ? ((S) < sizeof(N) * CHAR_BIT ? (N) << (S) : 0)                    \
            : (-(S) < sizeof(N) * CHAR_BIT ? (N) >> -(S) : 0))

// upper bound on the numer of base-10 digits in the greatest value of type
// `unsigned TYPE`. used to allocate `sprintf` buffers.
// an approximation of `floor(1 + log2((double)UTYPE_MAX) * log10(2.0))`
#define DIGS(TYPE) (1 + sizeof(TYPE) * CHAR_BIT * 7 / 23)

#if defined(SL16_LINEAR) == defined(SL16_QUADRATIC)
#error exactly one of `SL16_LINEAR, SL16_QUADRATIC` must be defined
#define SIGMA 0x0000
#define LOG_POLY(X) log_poly = 0
#define EXP_POLY(X) exp_poly = 0
#else

#ifdef SL16_LINEAR
#define SIGMA 0x000a               /* exp2(0.0430) */
#define LOG_POLY(X) log_poly = (X) /* x */
#define EXP_POLY(X) exp_poly = (X) /* x */
#endif

#ifdef SL16_QUADRATIC
#define SIGMA 0xfffe /* exp2(-0.005) */
#define LOG_POLY(X)                                                            \
  _x = (X), _x2 = (int32_t)_x * (int32_t)_x >> SL16_INT_OFF,                   \
  log_poly = _x + (_x - _x2 >> 2) + (_x - _x2 >> 3) /* 11/8 x - 3/8 x2 */
#define EXP_POLY(X)                                                            \
  _x = (X), _x2 = (int32_t)_x * (int32_t)_x >> SL16_INT_OFF,                   \
  exp_poly = _x + (_x2 - _x >> 2) + (_x2 - _x >> 3) /* 5/8 x + 3/8 x2 */
#endif

#endif

#define DEF_FROM_INTO(SUFFIX, DECL_SPEC)                                       \
  sl16_t sl16_fromu##SUFFIX(unsigned DECL_SPEC n) {                            \
    /* pack exponent and mantissa. piecewise-polynomial approximation of */    \
    /* return log2((double)n) * (1 << SL16_INT_OFF); */                        \
    if (n == 0)                                                                \
      return SL16_MIN;                                                         \
    int e = 0; /* codegens into a `lzcnt` instruction */                       \
    for (unsigned DECL_SPEC m = n; m >>= 1; e++)                               \
      ;                                                                        \
    int16_t LOG_POLY(SANE_SHIFT(n, SL16_INT_OFF - e) & ~SL16_INT_MASK);        \
    return ((uint16_t)e << SL16_INT_OFF | log_poly & SL16_FRAC_MASK) + SIGMA;  \
  }                                                                            \
                                                                               \
  unsigned DECL_SPEC sl16_intou##SUFFIX(sl16_t a) {                            \
    /* extract exponent and mantissa. piecewise-polynomial approximation of */ \
    /* return exp2((double)a / (1 << SL16_INT_OFF)); */                        \
    int16_t EXP_POLY((a -= SIGMA) & ~SL16_INT_MASK);                           \
    return SANE_SHIFT((unsigned DECL_SPEC)(exp_poly | 1 << SL16_INT_OFF),      \
                      ((int16_t)a >> SL16_INT_OFF) - SL16_INT_OFF);            \
  }                                                                            \
                                                                               \
  sl16_t sl16_from##SUFFIX(signed DECL_SPEC n) {                               \
    return n < 0 ? sl16_fromu##SUFFIX(-n) | SL16_SIGN_MASK                     \
                 : sl16_fromu##SUFFIX(n);                                      \
  }                                                                            \
                                                                               \
  signed DECL_SPEC sl16_into##SUFFIX(sl16_t a) {                               \
    return a & SL16_SIGN_MASK ? -sl16_intou##SUFFIX(a & ~SL16_SIGN_MASK)       \
                              : sl16_intou##SUFFIX(a);                         \
  }

DEF_FROM_INTO(c, char)
DEF_FROM_INTO(s, short)
DEF_FROM_INTO(, int)
DEF_FROM_INTO(l, long)
DEF_FROM_INTO(ll, long long)

#undef DEF_FROM_INTO

#undef SIGMA
#undef LOG_POLY
#undef EXP_POLY

// remember that `sl16_t` stores values wrapped in logarithms and most of these
// functions should be self-explanatory

sl16_t sl16_add(sl16_t a, sl16_t b) {
  sl16_t e_max = ((int16_t)a > (int16_t)b ? a : b) & SL16_INT_MASK;
  e_max -= 1 << SL16_INT_OFF; // avoid losing precision to the sign bit
  return sl16_log2(sl16_exp2(a - e_max) + sl16_exp2(b - e_max)) + e_max;
}
sl16_t sl16_sub(sl16_t a, sl16_t b) { return sl16_add(a, b ^ SL16_SIGN_MASK); }
sl16_t sl16_neg(sl16_t a) { return a ^ SL16_SIGN_MASK; }
sl16_t sl16_mul(sl16_t a, sl16_t b) { return a + b; }
sl16_t sl16_div(sl16_t a, sl16_t b) { return a - b; }
sl16_t sl16_inv(sl16_t a) { return -a; /* notice the sign bit is preserved */ }
// we store logarithms in fixed point but `sl16_from` and `sl16_into` operate on
// integers, so just remember to bit-shift back and forth
sl16_t sl16_log2(sl16_t a) {
  return sl16_froms(a) - (SL16_INT_OFF << SL16_INT_OFF);
}
sl16_t sl16_exp2(sl16_t a) {
  return sl16_intos(a + (SL16_INT_OFF << SL16_INT_OFF)) & ~SL16_SIGN_MASK;
}
sl16_t sl16_log(sl16_t a) { return sl16_log2(a) + SL16_LOG2; }
sl16_t sl16_exp(sl16_t a) { return sl16_exp2(a - SL16_LOG2); }
sl16_t sl16_powi(sl16_t a, int n) { return (int16_t)a * n; }
sl16_t sl16_rooti(sl16_t a, int n) { return (int16_t)a / n & ~SL16_SIGN_MASK; }
sl16_t sl16_pow(sl16_t a, sl16_t b) {
  return (int32_t)(int16_t)a * sl16_exp2(b) >> SL16_INT_OFF & ~SL16_SIGN_MASK;
}
sl16_t sl16_root(sl16_t a, sl16_t b) { return sl16_pow(a, -b); }
sl16_t sl16_square(sl16_t a) { return a << 1; }
sl16_t sl16_cube(sl16_t a) { return a + (a << 1); }
// trust that the compiler has good codegen for division by 2 and 3
sl16_t sl16_sqrt(sl16_t a) { return (int16_t)a / 2 & ~SL16_SIGN_MASK; }
sl16_t sl16_cbrt(sl16_t a) { return (int16_t)a / 3 & ~SL16_SIGN_MASK; }
sl16_t sl16_min(sl16_t a, sl16_t b) {
  return SL16_MONOTONIC(a) < SL16_MONOTONIC(b) ? a : b;
}
sl16_t sl16_max(sl16_t a, sl16_t b) {
  return SL16_MONOTONIC(a) > SL16_MONOTONIC(b) ? a : b;
}
sl16_t sl16_hypot(sl16_t a, sl16_t b) {
  return sl16_sqrt(sl16_add(sl16_square(a), sl16_square(b)));
}
sl16_t sl16_abs(sl16_t a) { return a & ~SL16_SIGN_MASK; }

char *sl16_fmt_f(sl16_t a, char *flags, int prec) {
  unsigned long long exp10 = 1;
  for (int p = prec; p--; exp10 *= 10)
    if (exp10 > ULLONG_MAX / 10)
      return NULL; // additional iteration would overflow `exp10`

  bool plus = strchr(flags, '+'), space = strchr(flags, ' '),
       hash = strchr(flags, '#');
  char *sign = a & SL16_SIGN_MASK ? "-" : plus ? "+" : space ? " " : "";
  char *fmt = prec ? "%s%llu.%0*llu" : hash ? "%s%llu." : "%s%llu";
  unsigned long long n = sl16_intoull(a + 0x0352 /* 10 */ * prec);

  static char buf[DIGS(unsigned long long) + sizeof("-.")];
  snprintf(buf, sizeof(buf), fmt, sign, n / exp10, prec, n % exp10);
  return buf;
}

char *sl16_fmt_e(sl16_t a, char *flags, int prec) {
  unsigned long long exp5 = 1;
  for (int p = prec; p--; exp5 *= 5)
    if (exp5 > SANE_SHIFT(ULLONG_MAX, SL16_INT_OFF - prec) / SL16_FRAC_MASK / 5)
      return NULL; // additional iteration would overflow fractional part

  bool plus = strchr(flags, '+'), space = strchr(flags, ' '),
       hash = strchr(flags, '#');
  char *sign = a & SL16_SIGN_MASK ? "-" : plus ? "+" : space ? " " : "";
  char *fmt = prec ? "%s1p%c%hu.%0*llu" : hash ? "%s1p%c%hu." : "%s1p%c%hu";
  char e_sign = (int16_t)a < 0 ? '-' : '+';
  uint16_t e_abs = (int16_t)a < 0 ? -a : a;

  static char buf[DIGS(short) + DIGS(unsigned long long) + sizeof("-1p+.")];
  snprintf(buf, sizeof(buf), fmt, sign, e_sign, e_abs >> SL16_INT_OFF, prec,
           SANE_SHIFT((e_abs & SL16_FRAC_MASK) * exp5, prec - SL16_INT_OFF));
  return buf;
}
