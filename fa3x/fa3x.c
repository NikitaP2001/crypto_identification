#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <gmptools.h>

#include <x86intrin.h>

#include "fa3x.h"

static void fa3x_random(uint8_t *buffer, size_t length)
{
        while (length / 8 > 0) {
                uint64_t step_rnd = 0;
                while (_rdrand64_step(&step_rnd) == 0);
                *((uint64_t*)buffer) = step_rnd;
                buffer += 8;
                length -= 8;
        }
        while (length > 0) {
                uint16_t step_rnd = 0;
                while (_rdrand16_step(&step_rnd) == 0);
                *((uint8_t*)buffer) = (uint8_t)step_rnd;
                buffer += 1;
                length -= 1;
        }
}

void fa3x_controller_init(struct controller_params *params, uint32_t N)
{
        params->N = N;
        params->member_count = 0;

        fa3x_random(params->x, sizeof(fa3x_block));
}


static void fa3x_hash_superpos(fa3x_key_t result, const fa3x_block x, size_t power)
{
        struct sha256_state state = {0};
        fa3x_block block;
        memcpy(block, x, sizeof(fa3x_block));
        for (size_t i = 0; i < power; i++) {
                sha256_state_init(&state);
                sha256_process_x86(&state, block, sizeof(fa3x_block));
                memset(block, 0, sizeof(fa3x_block));
                memcpy(block, state.hash_data, sizeof(fa3x_key_t));
        }
        memcpy(result, block, sizeof(fa3x_key_t));
}


_Bool fa3x_controller_add_memeber(struct controller_params *cparams, struct member_params *mparams)
{
        if (cparams->member_count + 1 < cparams->N) {
                mparams->N = cparams->N;
                cparams->member_count += 1;
                fa3x_hash_superpos(mparams->id, cparams->x, cparams->member_count);
                return true;
        }
        return false;
}


void fa3x_member_random(fa3x_key_t mr)
{
        fa3x_random(mr, sizeof(fa3x_key_t));
}


void fa3x_member_hash(fa3x_key_t result, const fa3x_key_t left, const fa3x_key_t right)
{
        struct sha256_state state = {0};
        fa3x_block sha_block;
        memcpy(sha_block, left, sizeof(fa3x_key_t));
        memcpy(sha_block + sizeof(fa3x_key_t), right, sizeof(fa3x_key_t));
        sha256_state_init(&state);
        /* sha keysize * 2 = 256 * 2 = 512 = block size */
        sha256_process_x86(&state, sha_block, sizeof(fa3x_block));
        memcpy(result, state.hash_data, sizeof(fa3x_key_t));
}


void fa3x_member_in(struct member_params *params, fa3x_key_t r, fa3x_key_t in1, fa3x_key_t in2)
{
        fa3x_block temp = {0}; 
        size_t n_a = 0;

        while (_rdrand64_step(&n_a) == 0);
        n_a = (n_a % (params->N - 1)) + 1;

        fa3x_key_t round_id;
        memcpy(temp, params->id, sizeof(fa3x_key_t));
        fa3x_hash_superpos(round_id, temp, n_a);

        memcpy(temp, round_id, sizeof(fa3x_key_t));
        fa3x_member_hash(in1, round_id, r);
        fa3x_hash_superpos(in2, temp, params->N);
        fa3x_member_hash(in2, in2, r);
}


_Bool fa3x_member_auth(size_t N, const fa3x_key_t r, const fa3x_key_t vid, 
const fa3x_key_t in1, const fa3x_key_t in2)
{
        fa3x_block block = {0};
        fa3x_key_t vid_temp, hvidr;
        memcpy(vid_temp, vid, sizeof(fa3x_key_t));
        for (size_t i = 1; i < 2 * N; i++) {
                fa3x_member_hash(hvidr, vid_temp, r);
                if (memcmp(hvidr, in1, sizeof(fa3x_key_t)) == 0)
                        return true;
                memcpy(block, vid_temp, sizeof(fa3x_key_t));
                fa3x_hash_superpos(vid_temp, block, 1);
                fa3x_member_hash(hvidr, vid_temp, r);
                if (memcmp(hvidr, in2, sizeof(fa3x_key_t)) == 0)
                        return true;
        }
        return false;
}