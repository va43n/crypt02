#include "../lab3.h"

static int aes_encrypt(void);
static int aes_show(char* file_name, int power);
static int aes_write(char* file_name, char* text, int power);
static int read_key(uint8_t key[AES_KEY_BYTES]);

int perform_task1_aes(char* buffer, size_t buffer_size) {
  (void)buffer_size;

  char file_name[BUFFER_SIZE], text[BUFFER_SIZE];
  int power, mode;

  if (parse_task1_aes_input(buffer, BUFFER_SIZE, &mode, file_name, &power,
                            text) == FAILURE)
    return FAILURE;

  switch (mode) {
    case AES_ENCRYPT_OPTION_NUMBER:
      return aes_encrypt();
    case AES_SHOW_OPTION_NUMBER:
      return aes_show(file_name, power);
    case AES_WRITE_OPTION_NUMBER:
      return aes_write(file_name, text, power);
    default:
      return FAILURE;
  }
}

static int read_key(uint8_t key[AES_KEY_BYTES]) {
  int fd = open(KEY_FILE, O_RDONLY);
  if (fd == -1) {
    perror("open key");
    return FAILURE;
  }
  memset(key, 0, AES_KEY_BYTES);
  ssize_t n = read(fd, key, AES_KEY_BYTES);
  close(fd);
  if (n < 0) return FAILURE;
  return SUCCESS;
}

static int aes_encrypt(void) {
  uint8_t message[BUFFER_SIZE] = {0};
  uint8_t cipher[BUFFER_SIZE + AES_BLOCK_BYTES] = {0};
  uint8_t key[AES_KEY_BYTES] = {0};

  int fd_m = open(MESSAGE_FILE, O_RDONLY);
  if (fd_m == -1) {
    perror("open message");
    return FAILURE;
  }
  ssize_t m_bytes = read(fd_m, message, BUFFER_SIZE);
  close(fd_m);
  if (m_bytes < 0) m_bytes = 0;

  if (read_key(key) == FAILURE) return FAILURE;

  struct timespec t0, t1;
  clock_gettime(CLOCK_MONOTONIC, &t0);

  size_t out_len = 0;
  aes_encrypt_buffer(message, (size_t)m_bytes, cipher, &out_len, key);

  clock_gettime(CLOCK_MONOTONIC, &t1);
  double elapsed =
      (double)(t1.tv_sec - t0.tv_sec) + (double)(t1.tv_nsec - t0.tv_nsec) / 1e9;

  int fd_c = open(CIPHER_FILE, O_WRONLY | O_TRUNC);
  if (fd_c == -1) {
    perror("open cipher");
    return FAILURE;
  }
  write(fd_c, cipher, out_len);
  close(fd_c);

  printf("Encrypted %zd bytes -> %zu bytes in %.6f s.\n", m_bytes, out_len,
         elapsed);
  return SUCCESS;
}

static int aes_show(char* file_name, int power) {
  char* file_names[] = {MESSAGE_FILE, CIPHER_FILE, KEY_FILE};
  int file_names_number = 3;

  for (int i = 0; i < file_names_number; i++) {
    if (strcmp(file_name, AES_SHOW_ALL) != 0 &&
        strcmp(file_name, file_names[i]) != 0)
      continue;

    int fd = open(file_names[i], O_RDONLY);
    if (fd == -1) {
      perror("open");
      return FAILURE;
    }

    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    ssize_t len = read(fd, buffer, BUFFER_SIZE);
    close(fd);

    printf("\n====== %s ======\n", file_names[i]);
    if (power == 16)
      print_file_in_hex(buffer, (size_t)len);
    else if (power == 0)
      print_file_in_symbol(buffer, (size_t)len);
    printf("====== %s ======\n", file_names[i]);
  }

  return SUCCESS;
}

static int aes_write(char* file_name, char* text, int power) {
  int fd = open(file_name, O_WRONLY | O_TRUNC);
  if (fd == -1) {
    perror("open");
    return FAILURE;
  }

  size_t len = strlen(text);
  int res = SUCCESS;
  char text_result[BUFFER_SIZE];
  size_t text_size = len;

  if (power == 16)
    res = convert_hex_to_symbol(text, len, text_result, &text_size);
  else if (power == 0)
    memcpy(text_result, text, len);

  if (res == FAILURE) {
    close(fd);
    return FAILURE;
  }

  write(fd, text_result, text_size);
  close(fd);
  return SUCCESS;
}