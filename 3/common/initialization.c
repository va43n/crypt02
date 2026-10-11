#include "../lab3.h"

int create_files_if_needed(void) {
  int fd = open(MESSAGE_FILE, O_RDWR | O_CREAT, 0644);
  if (fd == -1) {
    perror("open message");
    return FAILURE;
  }
  close(fd);

  fd = open(CIPHER_FILE, O_RDWR | O_CREAT, 0644);
  if (fd == -1) {
    perror("open cipher");
    return FAILURE;
  }
  close(fd);

  fd = open(KEY_FILE, O_RDWR | O_CREAT, 0644);
  if (fd == -1) {
    perror("open key");
    return FAILURE;
  }
  close(fd);

  return SUCCESS;
}

void init_random(void) { srand((unsigned)time(NULL)); }