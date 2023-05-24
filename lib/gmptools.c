#include <stdlib.h>
#include <x86intrin.h>
#include <sys/time.h>

#include "gmptools.h"

void gmpt_import(mpz_t dest, const void *int_data, uint32_t count)
{
        mpz_import(dest, count, -1, 1, 0, 0, int_data);
}


uint8_t *gmpt_export(const mpz_t src, uint32_t *count)
{
        const uint32_t byte_base = 0x100;
        *count = mpz_sizeinbase(src, byte_base);
        if (*count % GMPT_PAD_SIZE != 0)
                *count = (*count / GMPT_PAD_SIZE + 1) * GMPT_PAD_SIZE;
        uint8_t *buffer = calloc(*count, sizeof(uint8_t));
        mpz_export(buffer, NULL, -1, 1, 0, 0, src);
        return buffer;
}


void gmpt_random(mpz_t random, size_t bitlen)
{
        const size_t dword_bits = sizeof(uint32_t) * CHAR_BIT;
        mpz_set_ui(random, 0);
        while (bitlen > 0) {
                uint32_t step_rnd = 0;
                while (_rdrand32_step(&step_rnd) == 0);

                size_t step_bits = (bitlen < dword_bits) ? bitlen : dword_bits;
                step_rnd >>= dword_bits - step_bits;
                bitlen -= step_bits;

                mpz_mul_2exp(random, random, step_bits);
                mpz_add_ui(random, random, step_rnd);
        }
}


/* get uniform random value 0 < random < mod */
void gmpt_rndmod(mpz_t random, mpz_t mod)
{
        do {
                size_t bitlen = mpz_sizeinbase(mod, 2);
                gmpt_random(random, bitlen);
        } while (mpz_cmp_ui(random, 0) == 0);
}


_Bool gmpt_isprime(mpz_t value)
{
        const int reps = 50;
        return mpz_probab_prime_p(value, reps) > 0;
}


void gmpt_sha256(mpz_t x, struct sha256_state *hash)
{
        uint32_t count = 0;
        uint8_t *buffer = gmpt_export(x, &count);
        sha256_state_init(hash);
        sha256_process_x86(hash, buffer, count);
        free(buffer);
}

double get_current_time() 
{
        struct timespec res = {0};
        clock_gettime(CLOCK_MONOTONIC, &res);
        double nsec = (double)res.tv_nsec * 1e-9;
        double time = (double)res.tv_sec + nsec;
	return time;
}