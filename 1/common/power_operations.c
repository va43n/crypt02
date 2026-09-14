#include "../lab1.h"

void print_file_in_binary(char* buffer, size_t buffer_size) {
  unsigned int number_of_bits_in_char = sizeof(char) * 8;
  for (size_t i = 0; i < buffer_size; i++) {
    for (int j = number_of_bits_in_char - 1; j >= 0; j--)
      printf("%d", (unsigned int)(unsigned char)buffer[i] >> j & 1);
  }
  printf("\n");
}

void print_file_in_hex(char* buffer, size_t buffer_size) {
  for (size_t i = 0; i < buffer_size; i++) {
    printf("%x", (unsigned int)(unsigned char)buffer[i]);
  }
  printf("\n");
}

void print_file_in_dec(char* buffer, size_t buffer_size) {
  unsigned int res = 0;
  for (size_t i = 0; i < buffer_size; i++) {
    res += (unsigned int)(unsigned char)buffer[i];
  }
  printf("%d\n", res);
}

void print_file_in_symbol(char* buffer, size_t buffer_size) {
  (void)buffer_size;
  printf("%s\n", buffer);
}

int convert_binary_to_symbol(char* binary, size_t buffer_size, char* symbols,
                             size_t* symbols_size) {
  char* binary_alphabet = "01";
  size_t alphabet_len = strlen(binary_alphabet);

  size_t number_of_symbols_in_char = 0;
  char binary_char = '\0';

  *symbols_size = 0;

  for (size_t i = 0; i < buffer_size; i++) {
    int is_binary = FALSE;
    for (size_t j = 0; j < alphabet_len; j++) {
      if (binary[i] == binary_alphabet[j]) {
        is_binary = TRUE;
        break;
      }
    }
    if (is_binary == FALSE) {
      printf("Your input should contain only these symbols: %s\n",
             binary_alphabet);
      return FAILURE;
    }

    binary_char = (binary_char << 1) | (binary[i] - '0');
    number_of_symbols_in_char++;
    if (number_of_symbols_in_char == 8) {
      symbols[*symbols_size] = binary_char;
      binary_char = '\0';

      (*symbols_size)++;
      number_of_symbols_in_char = 0;
    }
  }

  if (number_of_symbols_in_char != 0) {
    symbols[*symbols_size] = binary_char;

    (*symbols_size)++;
  }

  return SUCCESS;
}

int convert_dec_to_symbol(char* dec, size_t buffer_size, char* symbols,
                          size_t* symbols_size) {
  (void)buffer_size;

  int dec_num = atoi(dec);

  char binary[BUFFER_SIZE];
  long i = BUFFER_SIZE - 1;
  binary[i--] = '\0';
  if (dec_num == 0) {
    binary[i--] = '0';
  } else {
    while (dec_num > 0 && i >= 0) {
      binary[i--] = (dec_num & 1) + '0';
      dec_num >>= 1;
    }
  }

  char binary_res[BUFFER_SIZE];
  strcpy(binary_res, binary + i + 1);
  size_t len = strlen(binary_res);

  return convert_binary_to_symbol(binary_res, len, symbols, symbols_size);
}

int convert_hex_to_symbol(char* hex, size_t buffer_size, char* symbols,
                          size_t* symbols_size) {
  char* hex_alphabet = "0123456789abcdef";
  size_t alphabet_len = strlen(hex_alphabet);

  size_t number_of_symbols_in_char = 0;
  char hex_char = '\0';

  *symbols_size = 0;

  for (size_t i = 0; i < buffer_size; i++) {
    int is_hex = FALSE;
    size_t symbol_pos;
    for (symbol_pos = 0; symbol_pos < alphabet_len; symbol_pos++) {
      if (tolower(hex[i]) == hex_alphabet[symbol_pos]) {
        is_hex = TRUE;
        break;
      }
    }
    if (is_hex == FALSE) {
      printf("Your input should contain only these symbols: %s\n",
             hex_alphabet);
      return FAILURE;
    }

    hex_char = (hex_char << 4) | (symbol_pos);
    number_of_symbols_in_char++;
    if (number_of_symbols_in_char == 2) {
      symbols[*symbols_size] = hex_char;
      hex_char = '\0';

      (*symbols_size)++;
      number_of_symbols_in_char = 0;
    }
  }

  if (number_of_symbols_in_char != 0) {
    symbols[*symbols_size] = hex_char;

    (*symbols_size)++;
  }

  return SUCCESS;
}