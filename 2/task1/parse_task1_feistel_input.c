#include "../lab2.h"

int parse_task1_feistel_input(char* buffer, size_t buffer_size, int* mode,
                              char* file_name, int* power, char* user_data) {
  char string_mode[BUFFER_SIZE], string_power[BUFFER_SIZE];
  memset(file_name, 0, buffer_size);
  memset(user_data, 0, buffer_size);
  memset(string_mode, 0, buffer_size);
  memset(string_power, 0, buffer_size);

  int n = sscanf(buffer, "%s %s %s %s", string_mode, file_name, string_power,
                 user_data);

  if (strcmp(string_mode, FEISTEL_ENCRYPT_OPTION) == 0) {
    *mode = FEISTEL_ENCRYPT_OPTION_NUMBER;
    if (n != 1) {
      printf("ERROR: parse_task1_feistel_input - 'encrypt' takes no args.\n");
      return FAILURE;
    }
    return SUCCESS;
  } else if (strcmp(string_mode, FEISTEL_PICK_KEY_METHOD_OPTION) == 0) {
    *mode = FEISTEL_PICK_KEY_METHOD_OPTION_NUMBER;
    if (n != 2) {
      printf(
          "ERROR: parse_task1_feistel_input - pick_key takes exactly 1 arg "
          "(1 or 2).\n");
      return FAILURE;
    }
    *power = atoi(file_name);
    return SUCCESS;
  } else if (strcmp(string_mode, FEISTEL_PICK_FUNC_OPTION) == 0) {
    *mode = FEISTEL_PICK_FUNC_OPTION_NUMBER;
    if (n != 2) {
      printf(
          "ERROR: parse_task1_feistel_input - pick_func takes exactly 1 arg "
          "(1 or 2).\n");
      return FAILURE;
    }
    *power = atoi(file_name);
    return SUCCESS;
  } else if (strcmp(string_mode, FEISTEL_SHOW_OPTION) == 0) {
    *mode = FEISTEL_SHOW_OPTION_NUMBER;
    if (n != 3) {
      printf(
          "ERROR: parse_task1_feistel_input - show takes 3 args "
          "(<file> <power>).\n");
      return FAILURE;
    }
  } else if (strcmp(string_mode, FEISTEL_WRITE_OPTION) == 0) {
    *mode = FEISTEL_WRITE_OPTION_NUMBER;
    if (n != 4) {
      printf(
          "ERROR: parse_task1_feistel_input - write takes 4 args "
          "(<file> <power> <text>).\n");
      return FAILURE;
    }

    size_t space_counter = 0, start_pos;
    for (start_pos = 0; start_pos < buffer_size && space_counter < 3;
         start_pos++) {
      if (buffer[start_pos] == ' ') space_counter++;
    }
    strcpy(user_data, buffer + start_pos);
  } else {
    printf("ERROR: parse_task1_feistel_input - unknown mode '%s'.\n",
           string_mode);
    return FAILURE;
  }

  if (strcmp(file_name, MESSAGE_FILE) != 0 &&
      strcmp(file_name, CIPHER_FILE) != 0 && strcmp(file_name, KEY_FILE) != 0 &&
      !(strcmp(file_name, FEISTEL_SHOW_ALL) == 0 &&
        *mode == FEISTEL_SHOW_OPTION_NUMBER)) {
    printf("ERROR: parse_task1_feistel_input - wrong file name.\n");
    return FAILURE;
  }

  if (strcmp(string_power, BINARY) == 0)
    *power = 2;
  else if (strcmp(string_power, HEX) == 0)
    *power = 16;
  else if (strcmp(string_power, SYMBOL) == 0)
    *power = 0;
  else {
    printf("ERROR: parse_task1_feistel_input - wrong power '%s'.\n",
           string_power);
    return FAILURE;
  }

  return SUCCESS;
}