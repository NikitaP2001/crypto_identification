#include <stdint.h>

#define SHA256_STDWCNT 8

struct sha256_state {
        uint32_t hash_data[SHA256_STDWCNT];
};

void sha256_state_init(struct sha256_state *_state);

void sha256_process_x86(struct sha256_state *_state, const uint8_t data[], 
uint32_t length);

_Bool sha256_cmp(const struct sha256_state *first, 
const struct sha256_state *second);
