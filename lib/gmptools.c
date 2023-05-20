#include <stdlib.h>
#include <x86intrin.h>

#include <gmp.h>

#include "gmptools.h"

void gmpt_import(mpz_t dest, const void *int_data, uint32_t count)
{
        mpz_import(dest, count, 1, 1, 0, 0, int_data);
}


uint8_t *gmpt_export(const mpz_t src, uint32_t *count)
{
        const uint32_t byte_base = 0x100;
        *count = mpz_sizeinbase(src, byte_base);
        uint8_t *buffer = malloc(*count);
        mpz_export(buffer, NULL, 1, 1, 0, 0, src);
        return buffer;
}


void gmpt_random(mpz_t random, size_t bitlen)
{
        const size_t dword_bits = sizeof(uint32_t) * CHAR_BIT;
        while (bitlen > 0) {
                uint32_t step_rnd = 0;
                while (_rdseed32_step(&step_rnd) == 0);

                size_t step_bits = (bitlen < dword_bits) ? bitlen : dword_bits;
                step_rnd >>= dword_bits - step_bits;
                bitlen -= step_bits;

                mpz_mul_2exp(random, random, step_bits);
                mpz_add_ui(random, random, step_rnd);
        }
}