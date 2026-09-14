#include "../lab1.h"

int perform_task2(char *buffer, size_t buffer_size) {
  (void)buffer_size;

  int mode = BACK_EXIT_OPTION_NUMBER;
  if (get_mode_from_input(buffer, &mode) == FAILURE) return FAILURE;

  if (mode == BACK_EXIT_OPTION_NUMBER) return SUCCESS;
  if (mode == LFSR_ENCRYPT_OPTION_NUMBER) {
    return SUCCESS;
  } else if (mode == XOR_ENCRYPT_OPTION_NUMBER) {
    return SUCCESS;
  } else {
    printf("ERROR: mode is not found.\n");
    return FAILURE;
  }

  return SUCCESS;
}