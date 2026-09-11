#include "../lab1.h"

int get_user_input(char* buffer, size_t buffer_size) {
  memset(buffer, 0, buffer_size);

  if (fgets(buffer, buffer_size, stdin) == NULL) {
    fprintf(stderr, "ERROR: get_user_input - cannot get user input.\n");
    return FAILURE;
  }

  size_t len = strlen(buffer);
  if (buffer[len - 1] == '\n') buffer[len - 1] = '\0';

  return SUCCESS;
}

int get_mode_from_input(char* buffer, int* mode) {
  int n = sscanf(buffer, "%d", mode);

  if (n != 1) {
    printf("ERROR: get_mode_from_input - Cannot parse user input.\n");
    return FAILURE;
  }
  return SUCCESS;
}