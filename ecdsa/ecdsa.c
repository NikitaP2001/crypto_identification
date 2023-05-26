#include <stdlib.h>

#include <gmptools.h>
#include "ecdsa.h"

void ecdsa_init(struct ecdsa_params *params)
{
        mpz_init(params->q);
        mpz_init(params->p);
        mpz_init(params->a);
        mpz_init(params->b);
        mpz_init(params->x_G);
        mpz_init(params->y_G);
}


void ecdsa_keys_create(const struct ecdsa_params *params, struct member_keys *keys)
{
        gmpt_rndmod(keys->d, params->n);
}


void ecdsa_sign(const struct member_keys *keys, mpz_t message)
{
}


_Bool ecdsa_verify()
{

}


void ecdsa_keys_destroy(struct member_keys *keys)
{

}


void ecdsa_free(struct ecdsa_params *params)
{
        mpz_clear(params->q);
        mpz_clear(params->p);
        mpz_clear(params->a);
        mpz_clear(params->b);
        mpz_clear(params->x_G);
        mpz_clear(params->y_G);
}