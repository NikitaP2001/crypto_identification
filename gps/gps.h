#include <gmp.h>

struct schnorr_params {
        mpz_t p;
        mpz_t q;
        mpz_t g;
        size_t bitsize;
};

#define GPS_L_1024 1024
#define GPS_L_2048 2048
#define GPS_L_3072 3072

#define GPS_N_160 160
#define GPS_N_224 224
#define GPS_N_256 256 

_Bool gps_init(struct schnorr_params *params, size_t bit_size);

void gps_free(struct schnorr_params *params);

/* generate private key s and public key v from s */
void gps_user_keys(mpz_t s, mpz_t v, struct schnorr_params *params);

void gps_load_keys(mpz_t s, mpz_t v, struct schnorr_params *params);

void gps_preprocess(struct schnorr_params *params, mpz_t x, mpz_t r);

void gps_load_coupon(struct schnorr_params *params, mpz_t x, mpz_t r);

void gps_sign(mpz_t y, mpz_t r, mpz_t s, mpz_t e);

void gps_verify(struct schnorr_params *params, mpz_t x_ver, mpz_t y, mpz_t v, mpz_t e);