#include <gmp.h>

#define GQ_NLEN_1024 1024
#define GQ_NLEN_2048 2048
#define GQ_NLEN_3072 3072

#define GQ_EVALUE 65537

struct gq_params {

        mpz_t n;

        mpz_t p;
        mpz_t q;

        mpz_t e; 
};

struct member_keys {
        mpz_t x;
        mpz_t y;
        mpz_t e;
        mpz_t n;
};

_Bool gq_init(struct gq_params *params, size_t bit_size);

void gq_keys_create(struct gq_params *params, struct member_keys *mparams);

void gq_commitment(const struct member_keys *params, mpz_t a, mpz_t r);

void gq_solve(const struct member_keys *params, mpz_t z, const mpz_t r, const mpz_t c);

_Bool gq_verify(const mpz_t z, const mpz_t e, const mpz_t a, mpz_t y, const mpz_t c, const mpz_t n);

void gq_keys_destroy(struct member_keys *params);

void gq_free(struct gq_params *params);