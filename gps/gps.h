#include <stdint.h>

#include <gmp.h>

struct gps_params {
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
