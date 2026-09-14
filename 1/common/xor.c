#include "../lab1.h"

void calculate_xor(char* message, size_t message_size, char* key,
                   size_t key_size, char* cipher) {
  for (size_t i = 0; i < message_size; i++) {
    if (i < key_size)
      cipher[i] = message[i] ^ key[i];
    else
      cipher[i] = message[i];
  }
}