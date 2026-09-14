#include "../lab1.h"

int parse_task1_lfsr_input(char* buffer, size_t buffer_size, int* mode,
                           char* file_name, int* power, char* user_data) {
  char string_mode[BUFFER_SIZE], string_power[BUFFER_SIZE];
  memset(file_name, 0, buffer_size);
  memset(user_data, 0, buffer_size);
  memset(string_mode, 0, buffer_size);
  memset(string_power, 0, buffer_size);

  int n = sscanf(buffer, "%s %s %s %s", string_mode, file_name, string_power,
                 user_data);

  if (strcmp(string_mode, LFSR_ENCRYPT_OPTION) == 0) {
    *mode = LFSR_ENCRYPT_OPTION_NUMBER;

    if (n != 1) {
      printf(
          "ERROR: parse_task1_lfsr_input - With encrypt mode there should be "
          "only one "
          "parameter - '%s'.\n",
          LFSR_ENCRYPT_OPTION);
      return FAILURE;
    }

    return SUCCESS;
  } else if (strcmp(string_mode, LFSR_PICK_OPTION) == 0) {
    *mode = LFSR_PICK_OPTION_NUMBER;
    if (n != 2) {
      printf(
          "ERROR: parse_task1_lfsr_input - With pick mode there should be "
          "exactly 2 "
          "parameters including '%s' keyword.\n",
          LFSR_PICK_OPTION);
      return FAILURE;
    }
    *power = atoi(file_name);

    return SUCCESS;
  } else if (strcmp(string_mode, LFSR_SHOW_OPTION) == 0) {
    *mode = LFSR_SHOW_OPTION_NUMBER;
    if (n != 3) {
      printf(
          "ERROR: parse_task1_xor_input - With show mode there should be "
          "exactly 3 "
          "parameters including '%s' keyword.\n",
          LFSR_SHOW_OPTION);
      return FAILURE;
    }
  } else if (strcmp(string_mode, LFSR_WRITE_OPTION) == 0) {
    *mode = LFSR_WRITE_OPTION_NUMBER;
    if (n != 4) {
      printf(
          "ERROR: parse_task1_lfsr_input - With write mode there should be "
          "exactly "
          "4 parameters including '%s' keyword.\n",
          LFSR_WRITE_OPTION);
      return FAILURE;
    }

    size_t space_counter = 0, start_pos;
    for (start_pos = 0; start_pos < buffer_size && space_counter < 3;
         start_pos++) {
      if (buffer[start_pos] == ' ') space_counter++;
    }
    strcpy(user_data, buffer + start_pos);
  } else {
    printf("ERROR: parse_task1_lfsr_input - Invalid mode '%s'.\n", string_mode);
    return FAILURE;
  }

  if (strcmp(file_name, MESSAGE_FILE) != 0 &&
      strcmp(file_name, CIPHER_FILE) != 0 && strcmp(file_name, KEY_FILE) != 0) {
    printf("ERROR: parse_task1_lfsr_input - wrong file name.\n");
    return FAILURE;
  }
  if (strcmp(string_power, BINARY) == 0)
    *power = 2;
  else if (strcmp(string_power, HEX) == 0)
    *power = 16;
  else if (strcmp(string_power, SYMBOL) == 0 &&
           strcmp(file_name, KEY_FILE) != 0)
    *power = 0;
  else if (strcmp(string_power, DEC) == 0 && strcmp(file_name, KEY_FILE) == 0)
    *power = 10;
  else {
    printf("ERROR: parse_task1_lfsr_input - wrong power.\n");
    return FAILURE;
  }

  return SUCCESS;
}