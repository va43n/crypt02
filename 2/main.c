#include "lab2.h"

int main(void) {
  setvbuf(stdout, NULL, _IONBF, 0);

  init_random();
  if (create_files_if_needed() == FAILURE) return EXIT_FAILURE;

  char buffer[BUFFER_SIZE];
  int mode;

  while (TRUE) {
    STANDARD_MENU;

    if (get_user_input(buffer, BUFFER_SIZE) == FAILURE) return FAILURE;
    if (get_mode_from_input(buffer, &mode) == FAILURE) continue;

    if (mode == BACK_EXIT_OPTION_NUMBER) break;

    if (mode == ENCRYPTION_MODE_NUMBER) {
      while (TRUE) {
        FEISTEL_ENCRYPTION_MENU;
        if (get_user_input(buffer, BUFFER_SIZE) == FAILURE) return FAILURE;
        if (strcmp(buffer, BACK_EXIT_OPTION) == 0) break;
        if (perform_task1_feistel(buffer, BUFFER_SIZE) == FAILURE) continue;
      }
    } else if (mode == DECRYPTION_MODE_NUMBER) {
      DECRYPTION_MENU;
      if (get_user_input(buffer, BUFFER_SIZE) == FAILURE) return FAILURE;
      if (perform_task2(buffer, BUFFER_SIZE) == FAILURE) continue;
    } else if (mode == EXAMINE_MODE_NUMBER) {
      while (TRUE) {
        EXAMINE_MENU;
        if (get_user_input(buffer, BUFFER_SIZE) == FAILURE) return FAILURE;
        if (strcmp(buffer, BACK_EXIT_OPTION) == 0) break;
        if (perform_task3(buffer, BUFFER_SIZE) == FAILURE) continue;
      }
    } else {
      printf("ERROR: mode is not found.\n");
      continue;
    }
  }

  return EXIT_SUCCESS;
}