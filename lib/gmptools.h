#include <stdint.h>


void gmpt_import(mpz_t dest, const void *int_data, uint32_t count);

uint8_t *gmpt_export(const mpz_t src, uint32_t *count);

void gmpt_random(mpz_t random, size_t bitlen);