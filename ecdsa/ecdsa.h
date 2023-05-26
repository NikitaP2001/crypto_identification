#include <gmp.h>

/* 
 *      Elliptic-curve dsa, follows ansi standart x9.62 
 */

struct curve_point {
        mpz_t x;
        mpz_t y;
};

#define ECDSA_P256 256
#define ECDSA_P384 384
#define ECDSA_P521 521

struct ecdsa_params {

        mpz_t n;

        /* a field size*/
        mpz_t p;

        /* y^2 = x^3 + ax + b */
        mpz_t a;
        mpz_t b;

        /* basic point G */
        struct curve_point G;
};


struct member_keys {
        /* point - public key */
        struct curve_point Q;

        /* is a private key */
        mpz_t d;
};

void ecdsa_init(struct ecdsa_params *params, size_t p);

void ecdsa_keys_create(const struct ecdsa_params *params, struct member_keys *keys);

/* sign message e, to receive (r,s) signature */
void ecdsa_sign(const struct ecdsa_params *params, const struct member_keys *keys, const mpz_t e, mpz_t r, mpz_t s);

_Bool ecdsa_verify(const struct ecdsa_params *params, const struct curve_point Q, const mpz_t r, const mpz_t s, const mpz_t e);

void ecdsa_keys_destroy(struct member_keys *keys);

void ecdsa_free(struct ecdsa_params *params);