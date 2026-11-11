#include "primes.h"
#include <stdio.h>
#include <stdlib.h>

u128 small_primes[400];
int small_primes_count = 0;

void primes_init(void) {
  small_primes_count = 0;
  char is_comp[SMALL_PRIMES_LIMIT] = {0};
  for (int i = 2; i < SMALL_PRIMES_LIMIT; i++) {
    if (!is_comp[i]) {
      small_primes[small_primes_count++] = (u128)i;
      for (int j = i * i; j < SMALL_PRIMES_LIMIT; j += i)
        is_comp[j] = 1;
    }
  }
}

int is_prime(u128 p, int t) {
  if (p < 2)
    return 0;
  if (p == 2 || p == 3)
    return 1;
  if ((p & 1) == 0)
    return 0;

  for (int i = 0; i < small_primes_count; i++) {
    u128 p1 = small_primes[i];
    if (p1 * p1 > p)
      return 1;
    if (p == p1)
      return 1;
    if (p % p1 == 0)
      return 0;
  }

  u128 m = p - 1;
  int b = 0;
  while ((m & 1) == 0) {
    m >>= 1;
    b++;
  }
  if (t < MR_MIN_ROUNDS)
    t = MR_MIN_ROUNDS;

  for (int i = 0; i < t; i++) {
    u128 a = rand_u128() % (p - 3) + 2;
    u128 z = pow_mod(a, m, p);
    if (z == 1 || z == p - 1)
      continue;
    int is_prime = 0;
    for (int j = 0; j < b - 1; j++) {
      z = mul_mod(z, z, p);
      if (z == p - 1) {
        is_prime = 1;
        break;
      }
    }
    if (is_prime)
      return 1;
  }
  return 0;
}

u128 generate_prime(int bits, int t, u128 *iterations_out) {
  if (bits < 2)
    bits = 2;
  if (bits > 128)
    bits = 128;

  u128 mask = (bits == 128) ? (u128)-1 : (((u128)1 << bits) - 1);

  *iterations_out = 0;
  while (1) {
    (*iterations_out)++;
    u128 c = rand_u128() & mask;
    c |= ((u128)1 << (bits - 1));
    c |= 1;
    if (is_prime(c, t))
      return c;
  }
}

u128 print_primes_in_range(u128 lo, u128 hi, u128 *min_out, u128 *max_out) {
  if (lo > hi) {
    u128 tmp = lo;
    lo = hi;
    hi = tmp;
  }

  u128 count = 0;
  u128 cur = lo;
  char buf[64];

  while (1) {
    if (is_prime(cur, MR_MIN_ROUNDS)) {
      if (count == 0)
        *min_out = cur;
      *max_out = cur;
      u128_to_str(cur, buf);
      printf("%s", buf);
      count++;
      if (count % 5 == 0)
        printf("\n");
      else
        printf("\t");
    }
    if (cur == hi)
      break;
    cur++;
    while (cur < hi && (cur & 1) == 0 && cur != 2)
      cur++;
  }
  if (count % 5 != 0)
    printf("\n");
  return count;
}