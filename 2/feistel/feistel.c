#include "feistel.h"

#include <string.h>

#define LFSR8_POLY 0b00000011

#define LFSR16_POLY 0b0100000000000011
#define LFSR16_INIT 0b0100000000000011

unsigned char lfsr8_next(unsigned char* state) {
  unsigned char s = *state;
  unsigned char feedback = 0;
  for (int i = 0; i < 8; i++)
    if ((LFSR8_POLY >> i) & 1) feedback ^= (s >> i) & 1;

  *state = (unsigned char)((s >> 1) | (feedback << 7));
  return s & 1;
}

unsigned short lfsr16_next(unsigned short* state) {
  unsigned short s = *state;
  unsigned short feedback = 0;
  for (int i = 0; i < 16; i++)
    if ((LFSR16_POLY >> i) & 1) feedback ^= (s >> i) & 1;

  *state = (unsigned short)((s >> 1) | (feedback << 15));
  return s & 1;
}

static int get_key_bit(const unsigned char* key, int bit_pos) {
  int byte_idx = bit_pos / 8;
  int bit_in_byte = 7 - (bit_pos % 8);
  return (key[byte_idx] >> bit_in_byte) & 1;
}

static void set_bit_in_bytes(unsigned char* dst, int bit_pos, int value) {
  int byte_idx = bit_pos / 8;
  int bit_in_byte = 7 - (bit_pos % 8);
  if (value)
    dst[byte_idx] |= (1 << bit_in_byte);
  else
    dst[byte_idx] &= ~(1 << bit_in_byte);
}

static void gen_subkey_a(const unsigned char* key, int round,
                         unsigned char out[FEISTEL_HALF_BYTES]) {
  memset(out, 0, FEISTEL_HALF_BYTES);
  int start_bit = round - 1;
  int key_bits = FEISTEL_KEY_BYTES * 8;
  for (int i = 0; i < 32; i++) {
    int bit = get_key_bit(key, (start_bit + i) % key_bits);
    set_bit_in_bytes(out, i, bit);
  }
}

static void gen_subkey_b(const unsigned char* key, int round,
                         unsigned char out[FEISTEL_HALF_BYTES]) {
  memset(out, 0, FEISTEL_HALF_BYTES);
  int start_bit = round - 1;
  int key_bits = FEISTEL_KEY_BYTES * 8;

  unsigned char seed = 0;
  for (int i = 0; i < 8; i++) {
    int bit = get_key_bit(key, (start_bit + i) % key_bits);
    seed |= (unsigned char)(bit << (7 - i));
  }

  for (int i = 0; i < 32; i++) {
    int bit = lfsr8_next(&seed);
    set_bit_in_bytes(out, i, bit);
  }
}

void generate_subkeys(
    const unsigned char* key, const feistel_settings* settings,
    unsigned char subkeys[FEISTEL_ROUNDS][FEISTEL_HALF_BYTES]) {
  memset(subkeys, 0, FEISTEL_ROUNDS * FEISTEL_HALF_BYTES);
  for (int r = 1; r <= FEISTEL_ROUNDS; r++) {
    if (settings->key_method == 1)
      gen_subkey_a(key, r, subkeys[r - 1]);
    else
      gen_subkey_b(key, r, subkeys[r - 1]);
  }
}

static void round_func_a(const unsigned char* R, const unsigned char* K,
                         unsigned char out[FEISTEL_HALF_BYTES]) {
  (void)R;
  memcpy(out, K, FEISTEL_HALF_BYTES);
}

static void round_func_b(const unsigned char* R, const unsigned char* K,
                         unsigned char out[FEISTEL_HALF_BYTES]) {
  unsigned short state = LFSR16_INIT;
  unsigned char seq[FEISTEL_HALF_BYTES];
  memset(seq, 0, FEISTEL_HALF_BYTES);
  for (int i = 0; i < 32; i++) {
    int bit = lfsr16_next(&state);
    set_bit_in_bytes(seq, i, bit);
  }
  for (int i = 0; i < FEISTEL_HALF_BYTES; i++) out[i] = R[i] ^ seq[i] ^ K[i];
}

static void xor4(unsigned char* dst, const unsigned char* a,
                 const unsigned char* b) {
  for (int i = 0; i < FEISTEL_HALF_BYTES; i++) dst[i] = a[i] ^ b[i];
}

static void feistel_round(unsigned char* L, unsigned char* R,
                          const unsigned char* K,
                          const feistel_settings* settings) {
  unsigned char f_out[FEISTEL_HALF_BYTES];
  if (settings->func_type == 1)
    round_func_a(R, K, f_out);
  else
    round_func_b(R, K, f_out);

  unsigned char new_R[FEISTEL_HALF_BYTES];
  xor4(new_R, L, f_out);

  memcpy(L, R, FEISTEL_HALF_BYTES);
  memcpy(R, new_R, FEISTEL_HALF_BYTES);
}

static void process_block(
    const unsigned char* in, unsigned char* out, const unsigned char* key,
    const feistel_settings* settings, int decrypt,
    unsigned char round_outputs[FEISTEL_ROUNDS][FEISTEL_BLOCK_BYTES]) {
  unsigned char subkeys[FEISTEL_ROUNDS][FEISTEL_HALF_BYTES];
  generate_subkeys(key, settings, subkeys);

  unsigned char L[FEISTEL_HALF_BYTES], R[FEISTEL_HALF_BYTES];
  memcpy(L, in, FEISTEL_HALF_BYTES);
  memcpy(R, in + FEISTEL_HALF_BYTES, FEISTEL_HALF_BYTES);

  for (int r = 0; r < FEISTEL_ROUNDS; r++) {
    int idx = decrypt ? (FEISTEL_ROUNDS - 1 - r) : r;
    feistel_round(L, R, subkeys[idx], settings);

    if (round_outputs) {
      memcpy(round_outputs[r], L, FEISTEL_HALF_BYTES);
      memcpy(round_outputs[r] + FEISTEL_HALF_BYTES, R, FEISTEL_HALF_BYTES);
    }
  }

  memcpy(out, R, FEISTEL_HALF_BYTES);
  memcpy(out + FEISTEL_HALF_BYTES, L, FEISTEL_HALF_BYTES);
}

void feistel_encrypt_block(const unsigned char* in, unsigned char* out,
                           const unsigned char* key,
                           const feistel_settings* settings) {
  process_block(in, out, key, settings, 0, NULL);
}

void feistel_decrypt_block(const unsigned char* in, unsigned char* out,
                           const unsigned char* key,
                           const feistel_settings* settings) {
  process_block(in, out, key, settings, 1, NULL);
}

void feistel_encrypt_block_track(
    const unsigned char* in, unsigned char* out, const unsigned char* key,
    const feistel_settings* settings,
    unsigned char round_outputs[FEISTEL_ROUNDS][FEISTEL_BLOCK_BYTES]) {
  process_block(in, out, key, settings, 0, round_outputs);
}

void feistel_encrypt_buffer(const unsigned char* in, size_t in_len,
                            unsigned char* out, size_t* out_len,
                            const unsigned char* key,
                            const feistel_settings* settings) {
  size_t blocks = (in_len + FEISTEL_BLOCK_BYTES - 1) / FEISTEL_BLOCK_BYTES;
  if (in_len == 0) blocks = 0;
  *out_len = blocks * FEISTEL_BLOCK_BYTES;

  for (size_t b = 0; b < blocks; b++) {
    unsigned char block[FEISTEL_BLOCK_BYTES] = {0};
    size_t off = b * FEISTEL_BLOCK_BYTES;
    size_t take = in_len - off;
    if (take > FEISTEL_BLOCK_BYTES) take = FEISTEL_BLOCK_BYTES;
    memcpy(block, in + off, take);

    feistel_encrypt_block(block, out + off, key, settings);
  }
}

void feistel_decrypt_buffer(const unsigned char* in, size_t in_len,
                            unsigned char* out, size_t* out_len,
                            const unsigned char* key,
                            const feistel_settings* settings) {
  size_t blocks = in_len / FEISTEL_BLOCK_BYTES;
  *out_len = blocks * FEISTEL_BLOCK_BYTES;

  for (size_t b = 0; b < blocks; b++) {
    unsigned char block[FEISTEL_BLOCK_BYTES];
    memcpy(block, in + b * FEISTEL_BLOCK_BYTES, FEISTEL_BLOCK_BYTES);
    feistel_decrypt_block(block, out + b * FEISTEL_BLOCK_BYTES, key, settings);
  }
}