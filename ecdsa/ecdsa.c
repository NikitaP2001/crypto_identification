#include <stdbool.h>
#include <stdlib.h>

#include <gmptools.h>
#include "ecdsa.h"

static void curve_point_init(struct curve_point point)
{
        mpz_init(point.x);
        mpz_init(point.y);
}


static void curve_point_clear(struct curve_point point)
{
        mpz_clear(point.x);
        mpz_clear(point.y);
}


void ecdsa_init(struct ecdsa_params *params)
{
        mpz_init(params->q);
        mpz_init(params->p);
        mpz_init(params->a);
        mpz_init(params->b);
        curve_point_init(params->G);
}


/* see https://github.com/masterzorag/ec_gmp */

void ecdsa_double(struct ecdsa_params *params, const struct curve_point p, struct curve_point *r)
{
	mpz_t slope, temp;
	mpz_init(temp);
	mpz_init(slope);
	
	if(mpz_cmp_ui(p.y, 0) != 0) {
		mpz_mul_ui(temp, p.y, 2);
		mpz_invert(temp, temp, params->p);
		mpz_mul(slope, p.x, p.x);
		mpz_mul_ui(slope, slope, 3);
		mpz_add(slope, slope, params->a);
		mpz_mul(slope, slope, temp);
		mpz_mod(slope, slope, params->p);
		mpz_mul(r->x, slope, slope);
		mpz_sub(r->x, r->x, p.x);
		mpz_sub(r->x, r->x, p.x);
		mpz_mod(r->x, r->x, params->p);
		mpz_sub(temp, p.x, r->x);
		mpz_mul(r->y, slope, temp);
		mpz_sub(r->y, r->y, p.y);
		mpz_mod(r->y, r->y, params->p);
	} else {
		mpz_set_ui(r->x, 0);
		mpz_set_ui(r->y, 0);
	}
	mpz_clear(temp);
	mpz_clear(slope);
}

void ecdsa_add(const struct ecdsa_params *params, const struct curve_point p, const struct curve_point q, struct curve_point *r)
{
	mpz_mod(p.x, p.x, params->p);
	mpz_mod(p.y, p.y, params->p);
	mpz_mod(q.x, q.x, params->p);
	mpz_mod(q.y, q.y, params->p);

	if(mpz_cmp_ui(p.x, 0) == 0 && mpz_cmp_ui(p.y, 0) == 0) {
		mpz_set(r->x, q.x);
		mpz_set(r->y, q.y);
		return;
	}

	if(mpz_cmp_ui(q.x, 0) == 0 && mpz_cmp_ui(q.y, 0) == 0) {
		mpz_set(r->x, p.x);
		mpz_set(r->y, p.y);
		return;
	}

	mpz_t temp;
	mpz_init(temp);

	if(mpz_cmp_ui(q.y, 0) != 0) { 
		mpz_sub(temp, params->p, q.y);
		mpz_mod(temp, temp, params->p);
	} else
		mpz_set_ui(temp, 0);

	if(mpz_cmp(p.y, temp) == 0 && mpz_cmp(p.x, q.x) == 0) {
		mpz_set_ui(r->x, 0);
		mpz_set_ui(r->y, 0);
		mpz_clear(temp);
		return;
	}
	
	if(mpz_cmp(p.x, q.x) == 0 && mpz_cmp(p.y, q.y) == 0)	{
		ecdsa_double(params, p, r);
		
		mpz_clear(temp);
		return;		
	} else {
		mpz_t slope;
		mpz_init_set_ui(slope, 0);

		mpz_sub(temp, p.x, q.x);
		mpz_mod(temp, temp, params->p);
		mpz_invert(temp, temp, params->p);
		mpz_sub(slope, p.y, q.y);
		mpz_mul(slope, slope, temp);
		mpz_mod(slope, slope, params->p);
		mpz_mul(r->x, slope, slope);
		mpz_sub(r->x, r->x, p.x);
		mpz_sub(r->x, r->x, q.x);
		mpz_mod(r->x, r->x, params->p);
		mpz_sub(temp, p.x, r->x);
		mpz_mul(r->y, slope, temp);
		mpz_sub(r->y, r->y, p.y);
		mpz_mod(r->y, r->y, params->p);
		
		mpz_clear(temp);
		mpz_clear(slope);
		return;
	}
}

void ecdsa_mul(const struct ecdsa_params *params, struct curve_point p, struct curve_point *r, mpz_t m)
{
	struct curve_point Q, T;
	mpz_init(Q.x); mpz_init(Q.y);
	mpz_init(T.x); mpz_init(T.y);
	long no_of_bits, loop;
	
	no_of_bits = mpz_sizeinbase(m, 2);
	mpz_set_ui(r->x, 0);
	mpz_set_ui(r->y, 0);
	if(mpz_cmp_ui(m, 0) == 0)
		return;
		
	mpz_set(Q.x, p.x);
	mpz_set(Q.y, p.y);
	if(mpz_tstbit(m, 0) == 1){
		mpz_set(r->x, p.x);
		mpz_set(r->y, p.y);
	}

	for(loop = 1; loop < no_of_bits; loop++) {
		mpz_set_ui(T.x, 0);
		mpz_set_ui(T.y, 0);
		ecdsa_double(params, Q, &T);

		//gmp_printf("\n %Zd %Zd %Zd %Zd ", Q.x, Q.y, T.x, T.y);
		mpz_set(Q.x, T.x);
		mpz_set(Q.y, T.y);
		mpz_set(T.x, r->x);
		mpz_set(T.y, r->y);
		if(mpz_tstbit(m, loop))
			ecdsa_add(params, T, Q, r);
	}
	
	mpz_clear(Q.x); mpz_clear(Q.y);
	mpz_clear(T.x); mpz_clear(T.y);
}	


void ecdsa_keys_create(const struct ecdsa_params *params, struct member_keys *keys)
{
        mpz_init(keys->d);
        curve_point_init(keys->Q);
        gmpt_rndmod(keys->d, params->n);
        ecdsa_mul(params, params->G, &keys->Q, keys->d);
}


void ecdsa_sign(const struct ecdsa_params *params, const struct member_keys *keys, const mpz_t e, mpz_t r, mpz_t s)
{
        mpz_t k;
        struct curve_point kG;
        mpz_init(k);
        curve_point_init(kG);

        do {
                gmpt_rndmod(k, params->n);

                ecdsa_mul(params, params->G, &kG, k);
                mpz_mod(r, kG.x, params->n);
        } while (mpz_cmp_ui(r, 0) == 0);

        mpz_invert(k, k, params->n);
        mpz_mul(s, keys->d, r);
        mpz_add(s, s, e);
        mpz_mul(s, s, k);
        mpz_mod(s, s, params->n);

        curve_point_clear(kG);
        mpz_clear(k);
}


_Bool ecdsa_verify(const struct ecdsa_params *params, const struct curve_point Q, const mpz_t r, const mpz_t s, const mpz_t e)
{
        mpz_t w, u1, u2;
        _Bool result = false;

        if (mpz_cmp_ui(r, 0) <= 0 || mpz_cmp(r, params->n) >= 0 
        || mpz_cmp_ui(s, 0) <= 0 || mpz_cmp(s, params->n))
                return result;

        mpz_init(w);
        mpz_init(u1);
        mpz_init(u2);
        mpz_invert(w, s, params->n);
        mpz_mul(u1, e, w);
        mpz_mod(u1, u1, params->n);
        mpz_mul(u2, r, w);
        mpz_mod(u2, u2, params->n);

        struct curve_point X, u2Q;
        curve_point_init(X);
        curve_point_init(u2Q);
        ecdsa_mul(params, params->G, &X, u1);
        ecdsa_mul(params, Q, &u2Q, u2);
        ecdsa_add(params, X, u2Q, &X);

        ecdsa_add(params, X, X, &u2Q);
        if (mpz_cmp(X.x, u2Q.x) != 0 || mpz_cmp(X.y, u2Q.y) != 0) {
                mpz_mod(X.x, X.x, params->n);
                if (mpz_cmp(X.x, r) == 0)
                        result = true;
        }

        curve_point_clear(u2Q);
        curve_point_clear(X);
        mpz_clear(u1);
        mpz_clear(u2);
        mpz_clear(w);
        return result;
}


void ecdsa_keys_destroy(struct member_keys *keys)
{
        mpz_clear(keys->d);
        curve_point_clear(keys->Q);
}


void ecdsa_free(struct ecdsa_params *params)
{
        mpz_clear(params->q);
        mpz_clear(params->p);
        mpz_clear(params->a);
        mpz_clear(params->b);
        curve_point_clear(params->G);
}