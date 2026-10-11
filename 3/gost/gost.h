#ifndef GOST_H
#define GOST_H

#include <stddef.h>
#include <stdint.h>

#define GOST_BLOCK_BYTES 8
#define GOST_KEY_BYTES 32
#define GOST_ROUNDS 32

void gost_init(void);

void gost_encrypt_block(const uint8_t in[GOST_BLOCK_BYTES],
                        uint8_t out[GOST_BLOCK_BYTES],
                        const uint8_t key[GOST_KEY_BYTES]);

void gost_decrypt_block(const uint8_t in[GOST_BLOCK_BYTES],
                        uint8_t out[GOST_BLOCK_BYTES],
                        const uint8_t key[GOST_KEY_BYTES]);

void gost_encrypt_block_track(
    const uint8_t in[GOST_BLOCK_BYTES], uint8_t out[GOST_BLOCK_BYTES],
    const uint8_t key[GOST_KEY_BYTES],
    uint8_t states[GOST_ROUNDS + 1][GOST_BLOCK_BYTES]);

void gost_encrypt_buffer(const uint8_t* in, size_t in_len, uint8_t* out,
                         size_t* out_len, const uint8_t key[GOST_KEY_BYTES]);

void gost_decrypt_buffer(const uint8_t* in, size_t in_len, uint8_t* out,
                         size_t* out_len, const uint8_t key[GOST_KEY_BYTES]);

#endif