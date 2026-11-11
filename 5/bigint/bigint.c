#include "bigint.h"
#include <stdlib.h>
#include <string.h>

u128 add_mod(u128 a, u128 b, u128 m) {
  if (a >= m)
    a %= m;
  if (b >= m)
    b %= m;
  if (a >= m - b)
    return a - (m - b);
  return a + b;
}

u128 mul_mod(u128 a, u128 b, u128 m) {
  u128 r = 0;
  a %= m;
  while (b) {
    if (b & 1)
      r = add_mod(r, a, m);
    a = add_mod(a, a, m);
    b >>= 1;
  }
  return r;
}

u128 pow_mod(u128 base, u128 exp, u128 mod) {
  u128 result = 1;
  base %= mod;
  while (exp) {
    if (exp & 1)
      result = mul_mod(result, base, mod);
    base = mul_mod(base, base, mod);
    exp >>= 1;
  }
  return result;
}

u128 gcd_u128(u128 a, u128 b) {
  if (a < b) {
    u128 temp = a;
    a = b;
    b = temp;
  }
  while (b) {
    u128 t = a % b;
    a = b;
    b = t;
  }
  return a;
}

u128 rand_u128(void) {
  u128 r = 0;
  for (int i = 0; i < 4; i++)
    r = (r << 32) | (u128)(uint32_t)rand();
  return r;
}

void u128_to_str(u128 n, char *buf) {
  if (n == 0) {
    buf[0] = '0';
    buf[1] = '\0';
    return;
  }
  char tmp[64];
  int i = 0;
  while (n > 0) {
    tmp[i++] = (char)('0' + (int)(n % 10));
    n /= 10;
  }
  for (int j = 0; j < i; j++)
    buf[j] = tmp[i - 1 - j];
  buf[i] = '\0';
}

int u128_from_str(const char *s, u128 *out) {
  u128 v = 0;
  for (const char *p = s; *p; p++) {
    if (*p < '0' || *p > '9')
      return -1;
    v = v * 10 + (u128)(*p - '0');
  }
  *out = v;
  return 0;
}