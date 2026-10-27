#include "../lab4.h"

void print_file_in_binary(char *buffer, size_t buffer_size) {
  for (size_t i = 0; i < buffer_size; i++)
    for (int j = 7; j >= 0; j--)
      printf("%d", (unsigned int)(unsigned char)buffer[i] >> j & 1);
  printf("\n");
}

void print_file_in_hex(char *buffer, size_t buffer_size) {
  for (size_t i = 0; i < buffer_size; i++)
    printf("%02x", (unsigned int)(unsigned char)buffer[i]);
  printf("\n");
}

void print_file_in_symbol(char *buffer, size_t buffer_size) {
  for (size_t i = 0; i < buffer_size; i++) {
    char c = buffer[i];
    putchar((c >= 32 && c < 127) ? c : '.');
  }
  putchar('\n');
}

int convert_hex_to_symbol(char *hex, size_t buffer_size, char *symbols,
                          size_t *symbols_size) {
  static const char hex_alphabet[] = "0123456789abcdef";
  *symbols_size = 0;
  unsigned char hex_char = 0;
  size_t half = 0;

  for (size_t i = 0; i < buffer_size; i++) {
    int is_hex = FALSE;
    size_t pos = 0;
    for (; pos < sizeof(hex_alphabet) - 1; pos++) {
      if (tolower((unsigned char)hex[i]) == hex_alphabet[pos]) {
        is_hex = TRUE;
        break;
      }
    }
    if (!is_hex) {
      printf("ERROR: hex input should only contain 0-9, a-f.\n");
      return FAILURE;
    }

    hex_char = (unsigned char)((hex_char << 4) | pos);
    if (++half == 2) {
      symbols[(*symbols_size)++] = (char)hex_char;
      hex_char = 0;
      half = 0;
    }
  }
  return SUCCESS;
}