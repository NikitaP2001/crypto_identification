#include <stdbool.h>
#include <stdlib.h>

#include <gmptools.h>
#include "ecdsa.h"

#define P_256_P "115792089210356248762697446949407573530086143415290314195533631308867097853951"
#define P_256_N "115792089210356248762697446949407573529996955224135760342422259061068512044369"
#define P_256_A "115792089210356248762697446949407573530086143415290314195533631308867097853948"
#define P_256_B "41058363725152142129326129780047268409114441015993725554835256314039467401291"
#define P_256_X "48439561293906451759052585252797914202762949526041747995844080717082404635286"
#define P_256_Y "36134250956749795798585127919587881956611106672985015071877198253568414405109"

#define P_384_P "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffeffffffff00000000" \
		"00000000ffffffff"
#define P_384_N "ffffffffffffffffffffffffffffffffffffffffffffffffc7634d81f4372ddf581a0db248b0a77a" \
		"ecec196accc52973"
#define P_384_A "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffeffffffff00000000" \
		"00000000fffffffc"
#define P_384_B "b3312fa7e23ee7e4988e056be3f82d19181d9c6efe8141120314088f5013875ac656398d8a2ed19d" \
		"2a85c8edd3ec2aef"
#define P_384_X "aa87ca22be8b05378eb1c71ef320ad746e1d3b628ba79b9859f741e082542a385502f25dbf55296c" \
		"3a545e3872760ab7"
#define P_384_Y "3617de4a96262c6f5d9e98bf9292dc29f8f41dbd289a147ce9da3113b5f0b8c00a60b1ce1d7e819d" \
		"7a431d7c90ea0e5f"

#define P_521_P "01ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" \
		"ffffffffffffffffffffffffffffffffffffffffffffffffffff"
#define P_521_N "01fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffa51868783bf2f" \
		"966b7fcc0148f709a5d03bb5c9b8899c47aebb6fb71e91386409"
#define P_521_A "01ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" \
		"fffffffffffffffffffffffffffffffffffffffffffffffffffc"
#define P_521_B "0051953eb9618e1c9a1f929a21a0b68540eea2da725b99b315f3b8b489918ef109e156193951ec7e" \
		"937b1652c0bd3bb1bf073573df883d2c34f1ef451fd46b503f00"
#define P_521_X "00c6858e06b70404e9cd9e3ecb662395b4429c648139053fb521f828af606b4d3dbaa14b5e77efe7" \
		"5928fe1dc127a2ffa8de3348b3c1856a429bf97e7e31c2e5bd66"
#define P_521_Y "011839296a789a3bc0045c8a5fb42c7d1bd998f54449579b446817afbd17273e662c97ee72995ef4" \
		"2640c550b9013fad0761353c7086a272c24088be94769fd16650"

static const char *key_d_256 = "8807723079962413571428369049095742764069019342326380235355295272994239715875";
static const char *key_Qx_256 = "79098906326470030776173750935706630781723410316707508891473817056535653107538";
static const char *key_Qy_256 = "50846974804062314130454137732211846877570766435293569099752058893645907217162";

static const char *key_d_384 = "34732275202400671981061101396406573285907962913872687948300708947162343737656290" \
			       "340226268045199802628829192577306173";
static const char *key_Qx_384 = "32744847611050191520440976881823820443697266364548239720070563204095857898253741" \
			        "684767323529407430626830743062748752";
static const char *key_Qy_384 = "36496990838861804638708210344076023280000977808346130904648618287957875706890309" \
			        "304251843161668127509717271680395890";

static const char *key_d_521 = "32962052834113096711222949869882026417955458197424958625666411445057658557210382" \
			      "86214166782735416015995859914498263484770106170210514610762988788607567592079";
static const char *key_Qx_521 = "98176035378732651441602557077510360827224327537770839273365387684878934215633150" \
			       "0649495719969377662667378564087851235459202255254622945476242072345401722687";
static const char *key_Qy_521 = "58580060039979019029543826858776379985366998099912894428345349660159936476448546" \
			       "71987418263627869107276170911467040284621856414915643741150448885552876007502";

static void curve_point_init(struct curve_point *point)
{
        mpz_init(point->x);
        mpz_init(point->y);
}


static void curve_point_clear(struct curve_point *point)
{
        mpz_clear(point->x);
        mpz_clear(point->y);
}



void ecdsa_init(struct ecdsa_params *params, size_t p)
{
	params->bitsize = p;
        switch (p) {
        case ECDSA_P256:
                mpz_init_set_str(params->p, P_256_P, 10);
                mpz_init_set_str(params->n, P_256_N, 10);
                mpz_init_set_str(params->a, P_256_A, 10);
                mpz_init_set_str(params->b, P_256_B, 10);
                curve_point_init(&params->G);
                mpz_init_set_str(params->G.x, P_256_X, 10);
                mpz_init_set_str(params->G.y, P_256_Y, 10);
                break;
        case ECDSA_P384:
                mpz_init_set_str(params->p, P_384_P, 16);
                mpz_init_set_str(params->n, P_384_N, 16);
                mpz_init_set_str(params->a, P_384_A, 16);
                mpz_init_set_str(params->b, P_384_B, 16);
                curve_point_init(&params->G);
                mpz_init_set_str(params->G.x, P_384_X, 16);
                mpz_init_set_str(params->G.y, P_384_Y, 16);
                break;
        case ECDSA_P521:
                mpz_init_set_str(params->p, P_521_P, 16);
                mpz_init_set_str(params->n, P_521_N, 16);
                mpz_init_set_str(params->a, P_521_A, 16);
                mpz_init_set_str(params->b, P_521_B, 16);
                curve_point_init(&params->G);
                mpz_init_set_str(params->G.x, P_521_X, 16);
                mpz_init_set_str(params->G.y, P_521_Y, 16);
                break;
        }
        
}


/* see https://github.com/masterzorag/ec_gmp */

void ecdsa_double(const struct ecdsa_params *params, struct curve_point p, struct curve_point *r)
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

void ecdsa_add(const struct ecdsa_params *params, struct curve_point p, struct curve_point q, struct curve_point *r)
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

	if (mpz_cmp_ui(q.y, 0) != 0) { 
		mpz_sub(temp, params->p, q.y);
		mpz_mod(temp, temp, params->p);
	} else
		mpz_set_ui(temp, 0);

	if (mpz_cmp(p.y, temp) == 0 && mpz_cmp(p.x, q.x) == 0) {
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
        curve_point_init(&keys->Q);
        gmpt_rndmod(keys->d, params->n);
        ecdsa_mul(params, params->G, &keys->Q, keys->d);
}


void ecdsa_keys_load(const struct ecdsa_params *params, struct member_keys *keys)
{
	switch (params->bitsize) {
	case ECDSA_P256:
		mpz_init_set_str(keys->d, key_d_256, 10);
		curve_point_init(&keys->Q);
		mpz_set_str(keys->Q.x, key_Qx_256, 10);
		mpz_set_str(keys->Q.y, key_Qy_256, 10);
		break;
	case ECDSA_P384:
		mpz_init_set_str(keys->d, key_d_384, 10);
		curve_point_init(&keys->Q);
		mpz_set_str(keys->Q.x, key_Qx_384, 10);
		mpz_set_str(keys->Q.y, key_Qy_384, 10);
		break;
	case ECDSA_P521:
		mpz_init_set_str(keys->d, key_d_521, 10);
		curve_point_init(&keys->Q);
		mpz_set_str(keys->Q.x, key_Qx_521, 10);
		mpz_set_str(keys->Q.y, key_Qy_521, 10);
		break;
	default:
		perror("invalid bitsize");
		break;
	}
}


void ecdsa_sign(const struct ecdsa_params *params, const struct member_keys *keys, const mpz_t e, mpz_t r, mpz_t s)
{
        mpz_t k;
        struct curve_point kG;
        mpz_init(k);
        curve_point_init(&kG);

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

        curve_point_clear(&kG);
        mpz_clear(k);
}


_Bool ecdsa_verify(const struct ecdsa_params *params, const struct curve_point Q, const mpz_t r, const mpz_t s, const mpz_t e)
{
        mpz_t w, u1, u2;
        _Bool result = false;

        if (mpz_cmp_ui(r, 0) <= 0 || mpz_cmp(r, params->n) >= 0 
        || mpz_cmp_ui(s, 0) <= 0 || mpz_cmp(s, params->n) >= 0)
                return result;

        mpz_init(w);
        mpz_init(u1);
        mpz_init(u2);
        mpz_invert(w, s, params->n);
        mpz_mul(u1, e, w);
        mpz_mod(u1, u1, params->n);
        mpz_mul(u2, r, w);
        mpz_mod(u2, u2, params->n);

        struct curve_point X, u2Q, u1G;
        curve_point_init(&X);
        curve_point_init(&u2Q);
        curve_point_init(&u1G);
        ecdsa_mul(params, params->G, &u1G, u1);
        ecdsa_mul(params, Q, &u2Q, u2);
        ecdsa_add(params, u1G, u2Q, &X);

        ecdsa_add(params, X, X, &u2Q);
        if (mpz_cmp(X.x, u2Q.x) != 0 || mpz_cmp(X.y, u2Q.y) != 0) {
                mpz_mod(X.x, X.x, params->n);
                if (mpz_cmp(X.x, r) == 0)
                        result = true;
        }

        curve_point_clear(&u2Q);
        curve_point_clear(&u1G);
        curve_point_clear(&X);
        mpz_clear(u1);
        mpz_clear(u2);
        mpz_clear(w);
        return result;
}


void ecdsa_keys_destroy(struct member_keys *keys)
{
        mpz_clear(keys->d);
        curve_point_clear(&keys->Q);
}


void ecdsa_free(struct ecdsa_params *params)
{
        mpz_clear(params->p);
        mpz_clear(params->n);
        mpz_clear(params->a);
        mpz_clear(params->b);
        curve_point_clear(&params->G);
}