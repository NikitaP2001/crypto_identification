#include <gmp.h>

/* 
 *      Elliptic-curve dsa, follows ansi standart x9.62 
 */

struct ecdsa_params {

        mpz_t n;

        /* a field size*/
        mpz_t q;
        mpz_t p;

        /* y^2 = x^3 + ax + b */
        mpz_t a;
        mpz_t b;

        /* basic point G */
        mpz_t x_G;
        mpz_t y_G;
};

struct member_keys {
        /* point - public key */
        mpz_t x_Q;
        mpz_t y_Q;

        /* is a private key */
        mpz_t d;
};

void ecdsa_init(struct ecdsa_params *params);

void ecdsa_keys_create(const struct ecdsa_params *params, struct member_keys *keys);

void ecdsa_sign(const struct member_keys *keys, mpz_t message);

_Bool ecdsa_verify();

void ecdsa_keys_destroy(struct member_keys *keys);

void ecdsa_free(struct ecdsa_params *params);