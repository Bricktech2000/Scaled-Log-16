#include <stdint.h>

typedef uint16_t sl16_t;

// these are solely for self-documentation. if you change them, stuff will break
#define SL16_WIDTH 16         // xxxx xxxx xxxx xxxx
#define SL16_INT_OFF 8        // ---- ---| ---- ----
#define SL16_FRAC_OFF 1       // ---- ---- ---- --|-
#define SL16_SIGN_OFF 0       // ---- ---- ---- ---|
#define SL16_INT_MASK 0xff00  // 1111 1111 0000 0000
#define SL16_FRAC_MASK 0x00fe // 0000 0000 1111 1110
#define SL16_SIGN_MASK 0x0001 // 0000 0000 0000 0001

// generated with
// printf("%#.4x\n", (int16_t)(log2(n) * (1 << SL16_INT_OFF)) & 0xfffe);
#define SL16_MIN 0x8000   /* 0x1.0p-128 */
#define SL16_MAX 0x7ffe   /* 0x1.fep127 */
#define SL16_ONE 0x0000   /* 1.0 */
#define SL16_TWO 0x0100   /* 2.0 */
#define SL16_THREE 0x0194 /* 3.0 */
#define SL16_HALF 0xff00  /* 1.0/2.0 */
#define SL16_THIRD 0xfe6a /* 1.0/3.0 */
#define SL16_SQRT2 0x0080 /* sqrt(2.0) */
#define SL16_SQRT3 0x00ca /* sqrt(3.0) */
#define SL16_CBRT2 0x0054 /* cbrt(2.0) */
#define SL16_CBRT3 0x0086 /* cbrt(3.0) */
#define SL16_LOG2 0xff78  /* log(2.0) */
#define SL16_LOG3 0x0022  /* log(3.0) */
#define SL16_TAU 0x02a6   /* 8.0 * atan(1.0) */
#define SL16_PI 0x01a6    /* 4.0 * atan(1.0) */
#define SL16_E 0x0170     /* exp(1.0) */

#define SL16_MONOTONIC(A) /* for relational operators */                       \
  (uint16_t)((A) >> SL16_FRAC_OFF ^ ((A) & SL16_SIGN_MASK ? 0x0000 : 0xffff))

#define DECL_FROM_INTO(SUFFIX, DECL_SPEC)                                      \
  sl16_t sl16_fromu##SUFFIX(unsigned DECL_SPEC n);                             \
  unsigned DECL_SPEC sl16_intou##SUFFIX(sl16_t a);                             \
  sl16_t sl16_from##SUFFIX(signed DECL_SPEC n);                                \
  signed DECL_SPEC sl16_into##SUFFIX(sl16_t a);

DECL_FROM_INTO(c, char)
DECL_FROM_INTO(s, short)
DECL_FROM_INTO(, int)
DECL_FROM_INTO(l, long)
DECL_FROM_INTO(ll, long long)

#undef DECL_FROM_INTO

sl16_t sl16_add(sl16_t a, sl16_t b);
sl16_t sl16_sub(sl16_t a, sl16_t b);
sl16_t sl16_neg(sl16_t a);
sl16_t sl16_mul(sl16_t a, sl16_t b);
sl16_t sl16_div(sl16_t a, sl16_t b);
sl16_t sl16_inv(sl16_t a);
sl16_t sl16_log2(sl16_t a);
sl16_t sl16_exp2(sl16_t a);
sl16_t sl16_log(sl16_t a);
sl16_t sl16_exp(sl16_t a);
sl16_t sl16_powi(sl16_t a, int n);
sl16_t sl16_rooti(sl16_t a, int n);
sl16_t sl16_pow(sl16_t a, sl16_t b);
sl16_t sl16_root(sl16_t a, sl16_t b);
sl16_t sl16_square(sl16_t a);
sl16_t sl16_cube(sl16_t a);
sl16_t sl16_sqrt(sl16_t a);
sl16_t sl16_cbrt(sl16_t a);
sl16_t sl16_hypot(sl16_t a, sl16_t b);
sl16_t sl16_min(sl16_t a, sl16_t b);
sl16_t sl16_max(sl16_t a, sl16_t b);
sl16_t sl16_abs(sl16_t a);

char *sl16_fmt(sl16_t a);
