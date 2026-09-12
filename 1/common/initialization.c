#include "../lab1.h"

int create_files_if_needed(void) {
  int fd = open(MESSAGE_FILE, O_RDWR | O_CREAT, 0644);
  if (fd == -1) {
    perror("open1");
    return FAILURE;
  }
  close(fd);

  fd = open(CIPHER_FILE, O_RDWR | O_CREAT, 0644);
  if (fd == -1) {
    perror("open2");
    return FAILURE;
  }
  close(fd);

  fd = open(KEY_FILE, O_RDWR | O_CREAT, 0644);
  if (fd == -1) {
    perror("open3");
    return FAILURE;
  }
  close(fd);

  return SUCCESS;
}

void init_random(void) { srand(time(NULL)); }