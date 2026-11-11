#ifndef PRIMES_H
#define PRIMES_H

#include "../bigint/bigint.h"

#define SMALL_PRIMES_LIMIT 2000
#define MR_MIN_ROUNDS 5

extern u128 small_primes[400];
extern int small_primes_count;

void primes_init(void);
int is_prime(u128 n, int t);
u128 generate_prime(int bits, int t, u128 *iterations_out);
u128 print_primes_in_range(u128 lo, u128 hi, u128 *min_out, u128 *max_out);

#endif