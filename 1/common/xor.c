#include "../lab1.h"

void calculate_xor(char* message, char* key, char* cipher) {
  size_t m_len = strlen(message);
  size_t k_len = strlen(key);

  for (size_t i = 0; i < m_len; i++) {
    if (i < k_len)
      cipher[i] = message[i] ^ key[i];
    else
      cipher[i] = message[i];
  }
}