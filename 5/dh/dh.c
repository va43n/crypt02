#include "dh.h"
#include "../bigint/bigint.h"
#include "../primes/primes.h"
#include "../roots/roots.h"
#include <stdio.h>

static void print_val(const char *label, u128 val) {
  char buf[64];
  u128_to_str(val, buf);
  printf("%-4s = %s\n", label, buf);
}

void dh_run(int bits, int t) {
  char buf[64];

  u128 iter_n = 0;
  u128 n = generate_prime(bits, t, &iter_n);
  u128_to_str(iter_n, buf);
  printf("Generated prime n after %s iterations.\n", buf);

  u128 roots[MAX_PRIMITIVE_ROOTS];
  int found = 0;
  int result = find_primitive_roots(n, roots, &found);

  u128 g;
  if (result == 0 && found > 0) {
    g = roots[0];
  } else {
    printf("WARNING: cannot find primitive root (factoring n-1 failed).\n");
    printf("Falling back to g = 2 (may not be a primitive root).\n");
    g = 2;
  }

  u128 XA = rand_u128() % (n - 3) + 2;
  u128 XB = rand_u128() % (n - 3) + 2;

  u128 YA = pow_mod(g, XA, n);
  u128 YB = pow_mod(g, XB, n);
  u128 KA = pow_mod(YB, XA, n);
  u128 KB = pow_mod(YA, XB, n);

  print_val("n", n);
  print_val("g", g);
  print_val("XA", XA);
  print_val("XB", XB);
  print_val("YA", YA);
  print_val("YB", YB);
  print_val("KA", KA);
  print_val("KB", KB);

  if (KA == KB)
    printf("OK: KA == KB\n");
  else
    printf("ERROR: KA != KB\n");
}