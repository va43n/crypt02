#include "../lab1.h"

int parse_task1_xor_input(char* buffer, size_t buffer_size, int* mode,
                          char* file_name, int* power, char* user_data) {
  char string_mode[BUFFER_SIZE], string_power[BUFFER_SIZE];
  memset(file_name, 0, buffer_size);
  memset(user_data, 0, buffer_size);
  memset(string_mode, 0, buffer_size);
  memset(string_power, 0, buffer_size);

  int n = sscanf(buffer, "%s %s %s %s", string_mode, file_name, string_power,
                 user_data);

  if (strcmp(string_mode, XOR_ENCRYPT_OPTION) == 0) {
    *mode = XOR_ENCRYPT_OPTION_NUMBER;

    if (n != 1) {
      printf(
          "ERROR: parse_task1_xor_input - With encrypt mode there should be "
          "only one "
          "parameter - '%s'.\n",
          XOR_ENCRYPT_OPTION);
      return FAILURE;
    }

    return SUCCESS;
  } else if (strcmp(string_mode, XOR_GEN_OPTION) == 0) {
    *mode = XOR_GEN_OPTION_NUMBER;
    if (n != 2) {
      printf(
          "ERROR: parse_task1_xor_input - With gen mode there should be "
          "exactly 2 "
          "parameters including 'gen' keyword.\n");
      return FAILURE;
    }
  } else if (strcmp(string_mode, XOR_SHOW_OPTION) == 0) {
    *mode = XOR_SHOW_OPTION_NUMBER;
    if (n != 3) {
      printf(
          "ERROR: parse_task1_xor_input - With show mode there should be "
          "exactly 3 "
          "parameters including 'show' keyword.\n");
      return FAILURE;
    }
  } else if (strcmp(string_mode, XOR_WRITE_OPTION) == 0) {
    *mode = XOR_WRITE_OPTION_NUMBER;
    if (n != 4) {
      printf(
          "ERROR: parse_task1_xor_input - With write mode there should be "
          "exactly "
          "4 parameters including 'write' keyword.\n");
      return FAILURE;
    }

    size_t space_counter = 0, start_pos;
    for (start_pos = 0; start_pos < buffer_size && space_counter < 3;
         start_pos++) {
      if (buffer[start_pos] == ' ') space_counter++;
    }
    strcpy(user_data, buffer + start_pos);
  } else {
    printf("ERROR: parse_task1_xor_input - Invalid mode '%s'.\n", string_mode);
    return FAILURE;
  }

  if (strcmp(file_name, MESSAGE_FILE) != 0 &&
      strcmp(file_name, CIPHER_FILE) != 0 && strcmp(file_name, KEY_FILE) != 0 &&
      (*mode == XOR_SHOW_OPTION_NUMBER &&
       strcmp(file_name, XOR_SHOW_ALL) != 0)) {
    printf("ERROR: parse_task1_xor_input - wrong file name.\n");
    return FAILURE;
  }
  if (*mode == XOR_GEN_OPTION_NUMBER) return SUCCESS;
  if (strcmp(string_power, BINARY) == 0)
    *power = 2;
  else if (strcmp(string_power, HEX) == 0)
    *power = 16;
  else if (strcmp(string_power, SYMBOL) == 0)
    *power = 0;
  else {
    printf("ERROR: parse_task1_xor_input - wrong power.\n");
    return FAILURE;
  }

  return SUCCESS;
}