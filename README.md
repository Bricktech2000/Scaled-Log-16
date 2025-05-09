# Scaled Log 16

_A 16-bit fixed-point logarithmic number system_

Scaled Log 16 is a number system that behaves like the floating-point numbers but is implemented using integer arithmetic. It stores values as the logarithm of their magnitude as a two’s complement 8.7 fixed-point number followed by their sign in the rightmost bit. This means that multiplication, division, powers and roots generally become single instructions—`add`, `sub`, `imul` and `idiv` respectively—at the expense of addition and subtraction, which require calls to implementations of $\log_2$ and $\exp_2$.

For performance, $\log_2$ and $\exp_2$ are implemented as the piecewise linear approximations $\log_2 x \approx \lfloor \log_2 x \rfloor + x / \exp_2 \lfloor \log_2 x \rfloor - 1 + \sigma$ and $\exp_2 x \approx (\{x - \sigma\} + 1) \exp_2 \lfloor x - \sigma \rfloor$, where $\sigma = 0.0430$ is a tuning factor and where $\lfloor x - \sigma \rfloor$ and $\{x - \sigma\} := (x - \sigma) - \lfloor x - \sigma \rfloor$ are bitwise `and`s, $\lfloor \log_2 \cdot \rfloor$ is a `lzcnt`, and $\cdot \exp_2 \lfloor \cdot \rfloor$ is a `shl` or `sar`. For greater precision, you could swap out the approximations for lookup tables.

This library assumes that the implementation performs an arithmetic right shift when the left operand to `>>` has a signed type and a negative value.

Sanity-check against floating-point arithmetic with:

```sh
make bin/test && bin/test
```
