#ifndef ROOTS_H
#define ROOTS_H

#include "../bigint/bigint.h"

#define MAX_PRIMITIVE_ROOTS 100
#define MAX_FACTORS 200

u128 euler_phi(u128 n);
int find_primitive_roots(u128 n, u128 *roots, int *count);

#endif