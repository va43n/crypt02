#include "rsa.h"
#include <stdio.h>
#include <time.h>

void rsa_generate_keys(mpz_t e, mpz_t d, mpz_t n) {
  mpz_t p, q, phi;
  gmp_randstate_t state;

  mpz_inits(p, q, phi, NULL);
  gmp_randinit_default(state);
  gmp_randseed_ui(state, (unsigned long)time(NULL));

  mpz_urandomb(p, state, 256);
  mpz_setbit(p, 255);
  mpz_nextprime(p, p);

  mpz_urandomb(q, state, 256);
  mpz_setbit(q, 255);
  mpz_nextprime(q, q);

  gmp_printf("Generated p, q:\n%Zd\n%Zd\n", p, q);

  rsa_generate_keys_from_pq(p, q, e, d, n);

  mpz_clears(p, q, phi, NULL);
  gmp_randclear(state);
}

void rsa_generate_keys_from_pq(const mpz_t p_in, const mpz_t q_in, mpz_t e,
                               mpz_t d, mpz_t n) {
  mpz_t p, q, phi;
  mpz_inits(p, q, phi, NULL);

  mpz_set(p, p_in);
  mpz_set(q, q_in);

  mpz_mul(n, p, q);
  mpz_sub_ui(p, p, 1);
  mpz_sub_ui(q, q, 1);
  mpz_mul(phi, p, q);

  mpz_set_ui(e, 65537);
  if (mpz_invert(d, e, phi) == 0) {
    printf("ERROR: Failed to compute modular inverse.\n");
  }

  mpz_clears(p, q, phi, NULL);
}

void rsa_encrypt(mpz_t c, const mpz_t m, const mpz_t e, const mpz_t n) {
  mpz_powm(c, m, e, n);
}

void rsa_decrypt(mpz_t m, const mpz_t c, const mpz_t d, const mpz_t n) {
  mpz_powm(m, c, d, n);
}