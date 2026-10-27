#include "../lab4.h"

static int read_bytes(const char *name, uint8_t *buf, size_t cap, size_t *len);
static int read_key16(const char *name, uint8_t key[16]);
static int write_bytes(const char *name, const uint8_t *buf, size_t len);

int perform_task2(char *buffer, size_t buffer_size) {
  (void)buffer_size;

  int mode = 0;
  if (get_mode_from_input(buffer, &mode) == FAILURE)
    return FAILURE;
  if (mode == BACK_EXIT_OPTION_NUMBER)
    return SUCCESS;

  static uint8_t cipher[BUFFER_SIZE];
  static uint8_t message[BUFFER_SIZE + AES_BLOCK_BYTES];
  size_t c_len = 0;

  if (read_bytes(CIPHER_FILE, cipher, BUFFER_SIZE, &c_len) == FAILURE)
    return FAILURE;

  if (mode == MODE_CTR_NUMBER) {
    uint8_t key[16] = {0}, iv[16] = {0};
    if (read_key16(KEY_FILE, key) == FAILURE)
      return FAILURE;
    if (read_key16(INIT_FILE, iv) == FAILURE)
      return FAILURE;

    ctr_crypt_buffer(cipher, c_len, message, key, iv);
    if (write_bytes(MESSAGE_FILE, message, c_len) == FAILURE)
      return FAILURE;
    printf("CTR decrypted %zu bytes.\n", c_len);
    return SUCCESS;
  }

  if (mode == MODE_EDE_NUMBER) {
    uint8_t k1[16], k2[16], k3[16];
    if (read_key16(KEY1_FILE, k1) == FAILURE)
      return FAILURE;
    if (read_key16(KEY2_FILE, k2) == FAILURE)
      return FAILURE;
    if (read_key16(KEY3_FILE, k3) == FAILURE)
      return FAILURE;

    size_t out_len = 0;
    ede_decrypt_buffer(cipher, c_len, message, &out_len, k1, k2, k3);
    if (write_bytes(MESSAGE_FILE, message, out_len) == FAILURE)
      return FAILURE;
    printf("EDE decrypted %zu bytes.\n", c_len);
    return SUCCESS;
  }

  printf("ERROR: mode is not found.\n");
  return FAILURE;
}

static int read_bytes(const char *name, uint8_t *buf, size_t cap, size_t *len) {
  int fd = open(name, O_RDONLY);
  if (fd == -1) {
    perror(name);
    return FAILURE;
  }
  ssize_t n = read(fd, buf, cap);
  close(fd);
  if (n < 0)
    return FAILURE;
  *len = (size_t)n;
  return SUCCESS;
}

static int read_key16(const char *name, uint8_t key[16]) {
  memset(key, 0, 16);
  int fd = open(name, O_RDONLY);
  if (fd == -1) {
    perror(name);
    return FAILURE;
  }
  read(fd, key, 16);
  close(fd);
  return SUCCESS;
}

static int write_bytes(const char *name, const uint8_t *buf, size_t len) {
  int fd = open(name, O_WRONLY | O_TRUNC);
  if (fd == -1) {
    perror(name);
    return FAILURE;
  }
  if (len > 0)
    write(fd, buf, len);
  close(fd);
  return SUCCESS;
}