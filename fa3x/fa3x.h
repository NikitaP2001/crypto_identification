#include <stdint.h>

#include <gmp.h>

#include <sha256.h>

typedef uint8_t fa3x_block[SHA256_BLKSIZE];

struct controller_params {

        size_t N;

        fa3x_block x;

        size_t member_count;

};

typedef uint8_t fa3x_key_t[SHA256_KEYSIZE];

struct member_params {

        size_t N;

        fa3x_key_t id;
};

void fa3x_controller_init(struct controller_params *params, uint32_t N);

_Bool fa3x_controller_add_memeber(struct controller_params *cparams, 
struct member_params *mparams);

void fa3x_member_random(fa3x_key_t mr);

void fa3x_member_hash(fa3x_key_t result, const fa3x_key_t left, const fa3x_key_t right);

void fa3x_member_in(struct member_params *params, fa3x_key_t r, fa3x_key_t in1, fa3x_key_t in2);

_Bool fa3x_member_auth(size_t N, const fa3x_key_t r, const fa3x_key_t vid, 
const fa3x_key_t in1, const fa3x_key_t in2);