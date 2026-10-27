#ifndef CTR_H
#define CTR_H

#include "../aes/aes.h"
#include <stddef.h>
#include <stdint.h>

void ctr_crypt_buffer(const uint8_t *in, size_t in_len, uint8_t *out,
                      const uint8_t key[AES_KEY_BYTES],
                      const uint8_t iv[AES_BLOCK_BYTES]);

void ctr_gamma3(const uint8_t key[AES_KEY_BYTES],
                const uint8_t iv[AES_BLOCK_BYTES], uint8_t g1[AES_BLOCK_BYTES],
                uint8_t g2[AES_BLOCK_BYTES], uint8_t g3[AES_BLOCK_BYTES]);

#endif