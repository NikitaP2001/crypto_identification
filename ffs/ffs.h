#include <gmp.h>

#define FFS_NLEN_1024 1024
#define FFS_NLEN_2048 2048
#define FFS_NLEN_3072 3072

struct ffs_params {

        mpz_t n;

        mpz_t p;
        mpz_t q;

        size_t k;
        size_t t;
};

#define FFS_KMAX 4

struct ffs_member_params {

        mpz_t s_arr[FFS_KMAX];

        mpz_t i_arr[FFS_KMAX];

};


_Bool ffs_init(struct ffs_params *params, size_t bit_size);

void ffs_keys_create(struct ffs_member_params *params, mpz_t n);

void ffs_keys_destroy(struct ffs_member_params *params);

void ffs_commitment(mpz_t X, mpz_t R, mpz_t n);

void ffs_solve(struct ffs_member_params *params, mpz_t Y, mpz_t E, mpz_t R, mpz_t n);

_Bool ffs_verify(mpz_t i_arr[FFS_KMAX], mpz_t X, mpz_t Y, mpz_t E, mpz_t n);

void ffs_free(struct ffs_params *params);