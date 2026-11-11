#ifndef BIGINT_H
#define BIGINT_H

#include <stdint.h>

typedef unsigned __int128 u128;

u128 add_mod(u128 a, u128 b, u128 m);
u128 mul_mod(u128 a, u128 b, u128 m);
u128 pow_mod(u128 base, u128 exp, u128 mod);
u128 gcd_u128(u128 a, u128 b);
u128 rand_u128(void);

void u128_to_str(u128 n, char *buf);
int u128_from_str(const char *s, u128 *out);

#endif