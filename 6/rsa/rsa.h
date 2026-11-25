#ifndef RSA_H
#define RSA_H

#include "../constants.h"
#include <gmp.h>

void rsa_generate_keys(mpz_t e, mpz_t d, mpz_t n);
void rsa_generate_keys_from_pq(const mpz_t p_in, const mpz_t q_in, mpz_t e,
                               mpz_t d, mpz_t n);
void rsa_encrypt(mpz_t c, const mpz_t m, const mpz_t e, const mpz_t n);
void rsa_decrypt(mpz_t m, const mpz_t c, const mpz_t d, const mpz_t n);

#endif