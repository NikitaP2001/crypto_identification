#include <gmp.h>

struct schnorr_params {
        mpz_t p;
        mpz_t q;
        mpz_t g;
};

#define DSA_L_1024 1024
#define DSA_L_2048 2048
#define DSA_L_3072 3072

#define DSA_N_160 160
#define DSA_N_224 224
#define DSA_N_256 256 

_Bool schnorr_init(struct schnorr_params *params, size_t bit_size);

void schnorr_free(struct schnorr_params *params);

void schnorr_rndmod(mpz_t random, mpz_t mod);

/* generate private key s and public key v from s */
void schnorr_user_keys(mpz_t s, mpz_t v, struct schnorr_params *params);

void schnorr_preprocess(struct schnorr_params *params, mpz_t x, mpz_t r);

void schnorr_sign(struct schnorr_params *params, mpz_t y, mpz_t r, mpz_t s, mpz_t e);

void schnorr_verify(struct schnorr_params *params, mpz_t x_ver, mpz_t y, mpz_t v, mpz_t e);