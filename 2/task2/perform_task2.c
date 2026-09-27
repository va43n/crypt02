#include "../lab2.h"

static int decrypt_with_feistel(void);

int perform_task2(char* buffer, size_t buffer_size) {
  (void)buffer_size;

  int mode = BACK_EXIT_OPTION_NUMBER;
  if (get_mode_from_input(buffer, &mode) == FAILURE) return FAILURE;

  if (mode == BACK_EXIT_OPTION_NUMBER) return SUCCESS;

  if (mode == FEISTEL_ENCRYPT_OPTION_NUMBER) {
    return decrypt_with_feistel();
  }

  printf("ERROR: mode is not found.\n");
  return FAILURE;
}

static int decrypt_with_feistel(void) {
  unsigned char cipher[BUFFER_SIZE] = {0};
  unsigned char key[FEISTEL_KEY_BYTES] = {0};
  unsigned char message[BUFFER_SIZE] = {0};

  int fd_c = open(CIPHER_FILE, O_RDONLY);
  int fd_k = open(KEY_FILE, O_RDONLY);
  if (fd_c == -1 || fd_k == -1) {
    perror("open");
    if (fd_c != -1) close(fd_c);
    if (fd_k != -1) close(fd_k);
    return FAILURE;
  }

  ssize_t c_bytes = read(fd_c, cipher, BUFFER_SIZE);
  ssize_t k_bytes = read(fd_k, key, FEISTEL_KEY_BYTES);
  close(fd_c);
  close(fd_k);

  if (c_bytes < 0) c_bytes = 0;
  if (k_bytes < FEISTEL_KEY_BYTES) {
    for (int i = (int)(k_bytes > 0 ? k_bytes : 0); i < FEISTEL_KEY_BYTES; i++)
      key[i] = 0;
  }

  feistel_settings s = {current_key_method, current_func_type};

  size_t out_len = 0;
  feistel_decrypt_buffer(cipher, (size_t)c_bytes, message, &out_len, key, &s);

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