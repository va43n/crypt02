#ifndef FEISTEL_H
#define FEISTEL_H

#include <stdint.h>
#include <stdlib.h>

#define FEISTEL_BLOCK_BYTES 8
#define FEISTEL_HALF_BYTES 4
#define FEISTEL_KEY_BYTES 8
#define FEISTEL_ROUNDS 16

typedef struct {
  int key_method;
  int func_type;
} feistel_settings;

unsigned char lfsr8_next(unsigned char* state);

unsigned short lfsr16_next(unsigned short* state);

void generate_subkeys(
    const unsigned char* key, const feistel_settings* settings,
    unsigned char subkeys[FEISTEL_ROUNDS][FEISTEL_HALF_BYTES]);

void feistel_encrypt_block(const unsigned char* in, unsigned char* out,
                           const unsigned char* key,
                           const feistel_settings* settings);
void feistel_decrypt_block(const unsigned char* in, unsigned char* out,
                           const unsigned char* key,
                           const feistel_settings* settings);

void feistel_encrypt_block_track(
    const unsigned char* in, unsigned char* out, const unsigned char* key,
    const feistel_settings* settings,
    unsigned char round_outputs[FEISTEL_ROUNDS][FEISTEL_BLOCK_BYTES]);

void feistel_encrypt_buffer(const unsigned char* in, size_t in_len,
                            unsigned char* out, size_t* out_len,
                            const unsigned char* key,
                            const feistel_settings* settings);
void feistel_decrypt_buffer(const unsigned char* in, size_t in_len,
                            unsigned char* out, size_t* out_len,
                            const unsigned char* key,
                            const feistel_settings* settings);

#endif