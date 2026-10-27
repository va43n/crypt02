#include "ede.h"
#include <string.h>

void ede_encrypt_block(const uint8_t in[AES_BLOCK_BYTES],
                       uint8_t out[AES_BLOCK_BYTES],
                       const uint8_t k1[AES_KEY_BYTES],
                       const uint8_t k2[AES_KEY_BYTES],
                       const uint8_t k3[AES_KEY_BYTES]) {
  uint8_t t1[16], t2[16];
  aes_encrypt_block(in, t1, k1);
  aes_decrypt_block(t1, t2, k2);
  aes_encrypt_block(t2, out, k3);
}

void ede_decrypt_block(const uint8_t in[AES_BLOCK_BYTES],
                       uint8_t out[AES_BLOCK_BYTES],
                       const uint8_t k1[AES_KEY_BYTES],
                       const uint8_t k2[AES_KEY_BYTES],
                       const uint8_t k3[AES_KEY_BYTES]) {
  uint8_t t1[16], t2[16];
  aes_decrypt_block(in, t1, k3);
  aes_encrypt_block(t1, t2, k2);
  aes_decrypt_block(t2, out, k1);
}

void ede_encrypt_block_track(const uint8_t in[AES_BLOCK_BYTES],
                             uint8_t out[AES_BLOCK_BYTES],
                             const uint8_t k1[AES_KEY_BYTES],
                             const uint8_t k2[AES_KEY_BYTES],
                             const uint8_t k3[AES_KEY_BYTES],
                             uint8_t states[EDE_STATES][AES_BLOCK_BYTES]) {
  uint8_t t1[16], t2[16];
  uint8_t s1[AES_ROUNDS + 1][16];
  uint8_t s2[AES_ROUNDS + 1][16];
  uint8_t s3[AES_ROUNDS + 1][16];

  memcpy(states[0], in, 16);

  aes_encrypt_block_track(in, t1, k1, s1);
  for (int i = 1; i <= AES_ROUNDS; i++)
    memcpy(states[i], s1[i], 16);

  aes_decrypt_block_track(t1, t2, k2, s2);
  for (int i = 1; i <= AES_ROUNDS; i++)
    memcpy(states[AES_ROUNDS + i], s2[i], 16);

  aes_encrypt_block_track(t2, out, k3, s3);
  for (int i = 1; i <= AES_ROUNDS; i++)
    memcpy(states[2 * AES_ROUNDS + i], s3[i], 16);
}

void ede_encrypt_buffer(const uint8_t *in, size_t in_len, uint8_t *out,
                        size_t *out_len, const uint8_t k1[AES_KEY_BYTES],
                        const uint8_t k2[AES_KEY_BYTES],
                        const uint8_t k3[AES_KEY_BYTES]) {
  size_t blocks = (in_len + 15) / 16;
  if (in_len == 0)
    blocks = 0;
  *out_len = blocks * 16;

  for (size_t b = 0; b < blocks; b++) {
    uint8_t block[16] = {0};
    size_t off = b * 16;
    size_t take = in_len - off;
    if (take > 16)
      take = 16;
    memcpy(block, in + off, take);
    ede_encrypt_block(block, out + off, k1, k2, k3);
  }
}

void ede_decrypt_buffer(const uint8_t *in, size_t in_len, uint8_t *out,
                        size_t *out_len, const uint8_t k1[AES_KEY_BYTES],
                        const uint8_t k2[AES_KEY_BYTES],
                        const uint8_t k3[AES_KEY_BYTES]) {
  size_t blocks = in_len / 16;
  *out_len = blocks * 16;

  for (size_t b = 0; b < blocks; b++) {
    uint8_t block[16];
    memcpy(block, in + b * 16, 16);
    ede_decrypt_block(block, out + b * 16, k1, k2, k3);
  }
}