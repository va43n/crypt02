#include "../lab4.h"

static int ensure_file(const char *name) {
  int fd = open(name, O_RDWR | O_CREAT, 0644);
  if (fd == -1) {
    perror(name);
    return FAILURE;
  }
  close(fd);
  return SUCCESS;
}

int create_files_if_needed(void) {
  if (ensure_file(MESSAGE_FILE) == FAILURE)
    return FAILURE;
  if (ensure_file(CIPHER_FILE) == FAILURE)
    return FAILURE;
  if (ensure_file(KEY_FILE) == FAILURE)
    return FAILURE;
  if (ensure_file(INIT_FILE) == FAILURE)
    return FAILURE;
  if (ensure_file(KEY1_FILE) == FAILURE)
    return FAILURE;
  if (ensure_file(KEY2_FILE) == FAILURE)
    return FAILURE;
  if (ensure_file(KEY3_FILE) == FAILURE)
    return FAILURE;
  return SUCCESS;
}

void init_random(void) { srand((unsigned)time(NULL)); }