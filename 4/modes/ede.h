#ifndef EDE_H
#define EDE_H

#include "../aes/aes.h"
#include <stddef.h>
#include <stdint.h>

#define EDE_STATES 31

void ede_encrypt_block(const uint8_t in[AES_BLOCK_BYTES],
                       uint8_t out[AES_BLOCK_BYTES],
                       const uint8_t k1[AES_KEY_BYTES],
                       const uint8_t k2[AES_KEY_BYTES],
                       const uint8_t k3[AES_KEY_BYTES]);

void ede_decrypt_block(const uint8_t in[AES_BLOCK_BYTES],
                       uint8_t out[AES_BLOCK_BYTES],
                       const uint8_t k1[AES_KEY_BYTES],
                       const uint8_t k2[AES_KEY_BYTES],
                       const uint8_t k3[AES_KEY_BYTES]);

void ede_encrypt_block_track(const uint8_t in[AES_BLOCK_BYTES],
                             uint8_t out[AES_BLOCK_BYTES],
                             const uint8_t k1[AES_KEY_BYTES],
                             const uint8_t k2[AES_KEY_BYTES],
                             const uint8_t k3[AES_KEY_BYTES],
                             uint8_t states[EDE_STATES][AES_BLOCK_BYTES]);

void ede_encrypt_buffer(const uint8_t *in, size_t in_len, uint8_t *out,
                        size_t *out_len, const uint8_t k1[AES_KEY_BYTES],
                        const uint8_t k2[AES_KEY_BYTES],
                        const uint8_t k3[AES_KEY_BYTES]);

void ede_decrypt_buffer(const uint8_t *in, size_t in_len, uint8_t *out,
                        size_t *out_len, const uint8_t k1[AES_KEY_BYTES],
                        const uint8_t k2[AES_KEY_BYTES],
                        const uint8_t k3[AES_KEY_BYTES]);

#endif