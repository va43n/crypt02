#include "ctr.h"
#include <string.h>

static void ctr_increment(uint8_t counter[AES_BLOCK_BYTES]) {
  for (int i = AES_BLOCK_BYTES - 1; i >= 0; i--)
    if (++counter[i] != 0)
      break;
}

void ctr_crypt_buffer(const uint8_t *in, size_t in_len, uint8_t *out,
                      const uint8_t key[AES_KEY_BYTES],
                      const uint8_t iv[AES_BLOCK_BYTES]) {
  uint8_t counter[AES_BLOCK_BYTES];
  memcpy(counter, iv, AES_BLOCK_BYTES);

  uint8_t gamma[AES_BLOCK_BYTES];
  size_t full = in_len / AES_BLOCK_BYTES;
  size_t tail = in_len % AES_BLOCK_BYTES;

  for (size_t b = 0; b < full; b++) {
    aes_encrypt_block(counter, gamma, key);
    for (int i = 0; i < AES_BLOCK_BYTES; i++)
      out[b * AES_BLOCK_BYTES + i] = in[b * AES_BLOCK_BYTES + i] ^ gamma[i];
    ctr_increment(counter);
  }

  if (tail > 0) {
    aes_encrypt_block(counter, gamma, key);
    for (size_t i = 0; i < tail; i++)
      out[full * AES_BLOCK_BYTES + i] =
          in[full * AES_BLOCK_BYTES + i] ^ gamma[i];
  }
}

void ctr_gamma3(const uint8_t key[AES_KEY_BYTES],
                const uint8_t iv[AES_BLOCK_BYTES], uint8_t g1[AES_BLOCK_BYTES],
                uint8_t g2[AES_BLOCK_BYTES], uint8_t g3[AES_BLOCK_BYTES]) {
  uint8_t counter[AES_BLOCK_BYTES];
  memcpy(counter, iv, AES_BLOCK_BYTES);

  aes_encrypt_block(counter, g1, key);
  ctr_increment(counter);
  aes_encrypt_block(counter, g2, key);
  ctr_increment(counter);
  aes_encrypt_block(counter, g3, key);
}