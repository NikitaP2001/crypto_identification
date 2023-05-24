#ifndef _GMPTOOLS_H_
#define _GMPTOOLS_H_
#include <stdint.h>

#include <gmp.h>

#include "sha256.h"

#define GMPT_PAD_SIZE 64 
void gmpt_import(mpz_t dest, const void *int_data, uint32_t count);

uint8_t *gmpt_export(const mpz_t src, uint32_t *count);

void gmpt_random(mpz_t random, size_t bitlen);

void gmpt_rndmod(mpz_t random, mpz_t mod);

_Bool gmpt_isprime(mpz_t value);

void gmpt_sha256(mpz_t x, struct sha256_state *hash);

double get_current_time();

#endif /* _GMPTOOLS_H_ */