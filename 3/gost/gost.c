#include "gost.h"

#include <string.h>

static const uint8_t sbox[8][16] = {
    {4, 10, 9, 2, 13, 8, 0, 14, 6, 11, 1, 12, 7, 15, 5, 3},
    {14, 11, 4, 12, 6, 13, 15, 10, 2, 3, 8, 1, 0, 7, 5, 9},
    {5, 8, 1, 13, 10, 3, 4, 2, 14, 15, 12, 7, 6, 0, 9, 11},
    {7, 13, 10, 1, 0, 8, 9, 15, 14, 4, 6, 12, 11, 2, 5, 3},
    {6, 12, 7, 1, 5, 15, 13, 8, 4, 10, 9, 14, 0, 3, 11, 2},
    {4, 11, 10, 0, 7, 2, 1, 13, 3, 6, 8, 5, 9, 12, 15, 14},
    {13, 11, 4, 1, 3, 15, 5, 9, 0, 10, 14, 7, 6, 8, 2, 12},
    {1, 15, 13, 0, 5, 7, 10, 4, 9, 2, 3, 14, 6, 11, 8, 12}};

static uint8_t precomputed_sbox[4][256];

void gost_init(void) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 256; j++) {
      uint8_t lo = (uint8_t)(j & 0x0F);
      uint8_t hi = (uint8_t)((j >> 4) & 0x0F);
      precomputed_sbox[i][j] =
          (uint8_t)(sbox[2 * i][lo] | (sbox[2 * i + 1][hi] << 4));
    }
  }
}

static uint32_t f(uint32_t x, uint32_t subkey) {
  uint32_t y = x + subkey;

  uint32_t s = (uint32_t)precomputed_sbox[0][(y >> 0) & 0xFF] |
               ((uint32_t)precomputed_sbox[1][(y >> 8) & 0xFF] << 8) |
               ((uint32_t)precomputed_sbox[2][(y >> 16) & 0xFF] << 16) |
               ((uint32_t)precomputed_sbox[3][(y >> 24) & 0xFF] << 24);

  return (s << 11) | (s >> 21);
}

static uint32_t get_subkey(const uint8_t key[GOST_KEY_BYTES], int round) {
  int idx;
  if (round < 8)
    idx = round;
  else if (round < 16)
    idx = round - 8;
  else if (round < 24)
    idx = round - 16;
  else
    idx = 7 - (round - 24);

  uint32_t subkey;
  memcpy(&subkey, key + idx * 4, 4);
  return subkey;
}

static void gost_crypt(const uint8_t in[GOST_BLOCK_BYTES],
                       uint8_t out[GOST_BLOCK_BYTES],
                       const uint8_t key[GOST_KEY_BYTES],
                       uint8_t states[GOST_ROUNDS + 1][GOST_BLOCK_BYTES],
                       int encrypt) {
  uint32_t left, right;
  memcpy(&left, in, 4);
  memcpy(&right, in + 4, 4);

  if (states) memcpy(states[0], in, GOST_BLOCK_BYTES);

  for (int i = 0; i < GOST_ROUNDS; i++) {
    int round = encrypt ? i : (GOST_ROUNDS - 1 - i);
    uint32_t subkey = get_subkey(key, round);

    if (encrypt) {
      uint32_t new_left = right;
      uint32_t new_right = left ^ f(right, subkey);
      left = new_left;
      right = new_right;
    } else {
      uint32_t new_right = left;
      uint32_t new_left = right ^ f(left, subkey);
      left = new_left;
      right = new_right;
    }

    if (states) {
      memcpy(states[i + 1], &left, 4);
      memcpy(states[i + 1] + 4, &right, 4);
    }
  }

  memcpy(out, &left, 4);
  memcpy(out + 4, &right, 4);
}

void gost_encrypt_block(const uint8_t in[GOST_BLOCK_BYTES],
                        uint8_t out[GOST_BLOCK_BYTES],
                        const uint8_t key[GOST_KEY_BYTES]) {
  gost_crypt(in, out, key, NULL, 1);
}

void gost_decrypt_block(const uint8_t in[GOST_BLOCK_BYTES],
                        uint8_t out[GOST_BLOCK_BYTES],
                        const uint8_t key[GOST_KEY_BYTES]) {
  gost_crypt(in, out, key, NULL, 0);
}

void gost_encrypt_block_track(
    const uint8_t in[GOST_BLOCK_BYTES], uint8_t out[GOST_BLOCK_BYTES],
    const uint8_t key[GOST_KEY_BYTES],
    uint8_t states[GOST_ROUNDS + 1][GOST_BLOCK_BYTES]) {
  gost_crypt(in, out, key, states, 1);
}

void gost_encrypt_buffer(const uint8_t* in, size_t in_len, uint8_t* out,
                         size_t* out_len, const uint8_t key[GOST_KEY_BYTES]) {
  size_t blocks = (in_len + GOST_BLOCK_BYTES - 1) / GOST_BLOCK_BYTES;
  if (in_len == 0) blocks = 0;
  *out_len = blocks * GOST_BLOCK_BYTES;

  for (size_t b = 0; b < blocks; b++) {
    uint8_t block[GOST_BLOCK_BYTES] = {0};
    size_t off = b * GOST_BLOCK_BYTES;
    size_t take = in_len - off;
    if (take > GOST_BLOCK_BYTES) take = GOST_BLOCK_BYTES;
    memcpy(block, in + off, take);
    gost_encrypt_block(block, out + off, key);
  }
}

void gost_decrypt_buffer(const uint8_t* in, size_t in_len, uint8_t* out,
                         size_t* out_len, const uint8_t key[GOST_KEY_BYTES]) {
  size_t blocks = in_len / GOST_BLOCK_BYTES;
  *out_len = blocks * GOST_BLOCK_BYTES;

  for (size_t b = 0; b < blocks; b++) {
    uint8_t block[GOST_BLOCK_BYTES];
    memcpy(block, in + b * GOST_BLOCK_BYTES, GOST_BLOCK_BYTES);
    gost_decrypt_block(block, out + b * GOST_BLOCK_BYTES, key);
  }
}