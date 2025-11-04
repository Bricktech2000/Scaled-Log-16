# Scaled Log 16

_A 16‑bit fixed-point logarithmic number system_

Scaled Log 16 is a number system that behaves like the floating-point numbers but is implemented using integer arithmetic. It stores values as the logarithm of their magnitude as a two’s complement 8.7 fixed-point number followed by their sign in the rightmost bit. This means that multiplication, division, powers and roots generally become single instructions—`add`, `sub`, `imul` and `idiv` respectively—at the expense of addition and subtraction, which require calls to implementations of $\log_2$ and $\exp_2$.

$\log_2$ and $\exp_2$ are implemented as the piecewise polynomial approximations $\log_2 n - \sigma \approx \lfloor \log_2 n \rfloor + P_{\log}\ (n / \exp_2 \lfloor \log_2 n \rfloor)$ and $\exp_2 (a + \sigma) \approx \exp_2 \lfloor a \rfloor \cdot P_{\exp}\ (a - \lfloor a \rfloor)$, where $\lfloor a \rfloor$ and $a - \lfloor a \rfloor$ are bitwise `and`s, $\lfloor \log_2 n \rfloor$ is an `lzcnt`, and $\exp_2$s are `shl`s and `sar`s. If `SL16_LINEAR` is defined, the more performant linear polynomials $P_{\log}\ (x + 1) = x$ and $P_{\exp}\ x - 1 = x$ and the tuning parameter $\sigma = 0.0430$ are used. If `SL16_QUADRATIC` is defined, the more accurate quadratic polynomials $P_{\log}\ (x + 1) = \frac{11}{8}x + \frac{3}{8}x^2$ and $P_{\exp}\ x - 1 = \frac{5}{8}x + \frac{3}{8}x^2$ and the tuning parameter $\sigma = -0.0050$ are used. Exactly one of `SL16_LINEAR, SL16_QUADRATIC` must be defined.

This library assumes that the implementation performs an arithmetic right shift when the left operand to `>>` has a signed type and a negative value.

Sanity-check against floating-point arithmetic with:

```sh
make bin/linear && bin/linear # piecewise linear approximations, more performant
make bin/quadratic && bin/quadratic # piecewise quadratic approximations, more accurate
```
