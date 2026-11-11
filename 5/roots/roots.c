#include "roots.h"
#include "../primes/primes.h"
#include <stdio.h>

static u128 pollard_rho(u128 n) {
  if ((n & 1) == 0)
    return 2;
  while (1) {
    u128 c = rand_u128() % (n - 1) + 1;
    u128 x = rand_u128() % (n - 2) + 2;
    u128 y = x, d = 1;
    u128 iter = 0;
    while (d == 1) {
      if (++iter > 1000000000ULL)
        return 0;
      x = add_mod(mul_mod(x, x, n), c, n);
      y = add_mod(mul_mod(y, y, n), c, n);
      y = add_mod(mul_mod(y, y, n), c, n);
      u128 diff = (x > y) ? x - y : y - x;
      d = gcd_u128(diff, n);
    }
    if (d != n)
      return d;
  }
}

static int factorize_rec(u128 n, u128 *factors, int *count) {
  if (n == 1)
    return 0;
  if (is_prime(n, MR_MIN_ROUNDS)) {
    if (*count >= MAX_FACTORS)
      return -1;
    factors[(*count)++] = n;
    return 0;
  }
  for (int i = 0; i < small_primes_count; i++) {
    u128 p = small_primes[i];
    if (p * p > n)
      break;
    while (n % p == 0) {
      if (*count >= MAX_FACTORS)
        return -1;
      factors[(*count)++] = p;
      n /= p;
    }
  }
  if (n == 1)
    return 0;
  if (is_prime(n, MR_MIN_ROUNDS)) {
    if (*count >= MAX_FACTORS)
      return -1;
    factors[(*count)++] = n;
    return 0;
  }

  u128 f = pollard_rho(n);
  if (f == 0)
    return -1;
  if (factorize_rec(f, factors, count) < 0)
    return -1;
  if (factorize_rec(n / f, factors, count) < 0)
    return -1;
  return 0;
}

u128 euler_phi(u128 n) {
  if (n == 1)
    return 1;
  u128 factors[MAX_FACTORS];
  int count = 0;
  if (factorize_rec(n, factors, &count) < 0)
    return 0;

  u128 result = n;
  for (int i = 0; i < count; i++) {
    if (i > 0 && factors[i] == factors[i - 1])
      continue;
    result = result / factors[i] * (factors[i] - 1);
  }
  return result;
}

static int primitive_root_exists(u128 n) {
  if (n == 1)
    return 0;
  if (n == 2 || n == 4)
    return 1;
  if ((n & 1) == 0)
    n >>= 1;
  if ((n & 1) == 0)
    return 0;
  u128 factors[MAX_FACTORS];
  int count = 0;
  if (factorize_rec(n, factors, &count) < 0)
    return 0;
  for (int i = 1; i < count; i++)
    if (factors[i] != factors[0])
      return 0;
  return 1;
}

int find_primitive_roots(u128 n, u128 *roots, int *count) {
  *count = 0;
  if (!primitive_root_exists(n))
    return -2;

  u128 phi = euler_phi(n);
  if (phi == 0)
    return -1;

  u128 phi_factors[MAX_FACTORS];
  int fc = 0;
  if (factorize_rec(phi, phi_factors, &fc) < 0)
    return -1;

  u128 phi_primes[MAX_FACTORS];
  int pc = 0;
  for (int i = 0; i < fc; i++) {
    if (i > 0 && phi_factors[i] == phi_factors[i - 1])
      continue;
    phi_primes[pc++] = phi_factors[i];
  }

  for (u128 a = 2; a < n && *count < MAX_PRIMITIVE_ROOTS; a++) {
    if (gcd_u128(a, n) != 1)
      continue;
    int ok = 1;
    for (int i = 0; i < pc; i++) {
      if (pow_mod(a, phi / phi_primes[i], n) == 1) {
        ok = 0;
        break;
      }
    }
    if (ok)
      roots[(*count)++] = a;
  }
  return 0;
}