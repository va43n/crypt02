#include "lab1.h"

int main(void) {
  setvbuf(stdout, NULL, _IONBF, 0);

  if (create_files_if_needed() == FAILURE) return EXIT_FAILURE;

  char buffer[BUFFER_SIZE];
  int mode;
  while (TRUE) {
    STANDARD_MENU;

    if (get_user_input(buffer, BUFFER_SIZE) == FAILURE) return FAILURE;
    if (get_mode_from_input(buffer, &mode) == FAILURE) continue;

    if (mode == 0) break;
    if (mode == 1) {
      XOR_ENCRYPTION_MENU;
      if (get_user_input(buffer, BUFFER_SIZE) == FAILURE) return FAILURE;

      if (perform_task1(buffer, BUFFER_SIZE) == FAILURE) continue;

    } else {
      printf("ERROR: mode is not found.\n");
      continue;
    }
  }

  return EXIT_SUCCESS;
}