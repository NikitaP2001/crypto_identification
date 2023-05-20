#include <stdint.h>

#define SHA256_STDWCNT 8

void sha256_state_init(uint32_t state[SHA256_STDWCNT]);

void sha256_process_x86(uint32_t state[8], const uint8_t data[], uint32_t length);