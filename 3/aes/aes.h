#ifndef AES_H
#define AES_H

#include <stddef.h>
#include <stdint.h>

#define AES_BLOCK_BYTES 16
#define AES_KEY_BYTES 16
#define AES_ROUNDS 10

void aes_encrypt_block(const uint8_t in[AES_BLOCK_BYTES],
                       uint8_t out[AES_BLOCK_BYTES],
                       const uint8_t key[AES_KEY_BYTES]);

void aes_decrypt_block(const uint8_t in[AES_BLOCK_BYTES],
                       uint8_t out[AES_BLOCK_BYTES],
                       const uint8_t key[AES_KEY_BYTES]);

void aes_encrypt_block_track(const uint8_t in[AES_BLOCK_BYTES],
                             uint8_t out[AES_BLOCK_BYTES],
                             const uint8_t key[AES_KEY_BYTES],
                             uint8_t states[AES_ROUNDS + 1][AES_BLOCK_BYTES]);

void aes_encrypt_buffer(const uint8_t* in, size_t in_len, uint8_t* out,
                        size_t* out_len, const uint8_t key[AES_KEY_BYTES]);

void aes_decrypt_buffer(const uint8_t* in, size_t in_len, uint8_t* out,
                        size_t* out_len, const uint8_t key[AES_KEY_BYTES]);

#endif