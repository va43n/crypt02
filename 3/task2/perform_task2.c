#include "../lab3.h"

int perform_task2(char* buffer, size_t buffer_size) {
  (void)buffer_size;

  int mode = BACK_EXIT_OPTION_NUMBER;
  if (get_mode_from_input(buffer, &mode) == FAILURE) return FAILURE;
  if (mode == BACK_EXIT_OPTION_NUMBER) return SUCCESS;

  if (mode != AES_ENCRYPT_OPTION_NUMBER) {
    printf("ERROR: mode is not found.\n");
    return FAILURE;
  }

  uint8_t cipher[BUFFER_SIZE] = {0};
  uint8_t message[BUFFER_SIZE + AES_BLOCK_BYTES] = {0};
  uint8_t key[AES_KEY_BYTES] = {0};

  int fd_c = open(CIPHER_FILE, O_RDONLY);
  if (fd_c == -1) {
    perror("open cipher");
    return FAILURE;
  }
  ssize_t c_bytes = read(fd_c, cipher, BUFFER_SIZE);
  close(fd_c);
  if (c_bytes < 0) c_bytes = 0;

  int fd_k = open(KEY_FILE, O_RDONLY);
  if (fd_k == -1) {
    perror("open key");
    return FAILURE;
  }
  read(fd_k, key, AES_KEY_BYTES);
  close(fd_k);

  size_t out_len = 0;
  aes_decrypt_buffer(cipher, (size_t)c_bytes, message, &out_len, key);

  int fd_m = open(MESSAGE_FILE, O_WRONLY | O_TRUNC);
  if (fd_m == -1) {
    perror("open message");
    return FAILURE;
  }
  write(fd_m, message, out_len);
  close(fd_m);

  printf("Decrypted %zd bytes -> %zu bytes.\n", c_bytes, out_len);
  return SUCCESS;
}