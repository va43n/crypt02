#include "../lab4.h"

static int handle_ctr_encrypt(void);
static int handle_ede_encrypt(void);
static int handle_show(char *file_name, int power);
static int handle_write(char *file_name, char *text, int power);
static int is_valid_file(const char *name);
static int read_file(const char *name, uint8_t *buf, size_t cap, size_t *len);
static int read_key16(const char *name, uint8_t key[16]);
static int write_file_bytes(const char *name, const uint8_t *buf, size_t len);
// static int fill_buffer_padded(const uint8_t* src, size_t src_len,
//                               uint8_t* dst, size_t block_size);

int perform_task1(char *buffer, size_t buffer_size) {
  (void)buffer_size;

  char mode[BUFFER_SIZE], fname[BUFFER_SIZE], power[BUFFER_SIZE],
      text[BUFFER_SIZE];
  memset(mode, 0, sizeof(mode));
  memset(fname, 0, sizeof(fname));
  memset(power, 0, sizeof(power));
  memset(text, 0, sizeof(text));

  int n = sscanf(buffer, "%s %s %s %s", mode, fname, power, text);

  if (strcmp(mode, MODE_CTR) == 0 && n == 1)
    return handle_ctr_encrypt();
  if (strcmp(mode, MODE_EDE) == 0 && n == 1)
    return handle_ede_encrypt();

  if (strcmp(mode, MODE_SHOW) == 0) {
    if (n < 3) {
      printf("ERROR: show <file> <hex|symbol>\n");
      return FAILURE;
    }
    int p = (strcmp(power, HEX) == 0) ? 16 : 0;
    return handle_show(fname, p);
  }

  if (strcmp(mode, MODE_WRITE) == 0) {
    if (n < 4) {
      printf("ERROR: write <file> <hex|symbol> <text>\n");
      return FAILURE;
    }
    int p = (strcmp(power, HEX) == 0) ? 16 : 0;

    size_t sc = 0, start = 0;
    for (size_t i = 0; i < BUFFER_SIZE && sc < 3; i++)
      if (buffer[i] == ' ') {
        sc++;
        start = i + 1;
      }
    memset(text, 0, sizeof(text));
    strcpy(text, buffer + start);
    return handle_write(fname, text, p);
  }

  printf("ERROR: unknown mode '%s'.\n", mode);
  return FAILURE;
}

static int is_valid_file(const char *name) {
  static const char *files[] = {MESSAGE_FILE, CIPHER_FILE, KEY_FILE, INIT_FILE,
                                KEY1_FILE,    KEY2_FILE,   KEY3_FILE};
  for (size_t i = 0; i < sizeof(files) / sizeof(files[0]); i++)
    if (strcmp(name, files[i]) == 0)
      return TRUE;
  return FALSE;
}

static int read_file(const char *name, uint8_t *buf, size_t cap, size_t *len) {
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

static int write_file_bytes(const char *name, const uint8_t *buf, size_t len) {
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

// static int fill_buffer_padded(const uint8_t* src, size_t src_len,
//                               uint8_t* dst, size_t block_size) {
//   size_t padded_len = ((src_len + block_size - 1) / block_size) * block_size;
//   if (src_len == 0) padded_len = 0;
//   memset(dst, 0, padded_len);
//   memcpy(dst, src, src_len);
//   return (int)padded_len;
// }

static int handle_ctr_encrypt(void) {
  static uint8_t message[BUFFER_SIZE];
  static uint8_t cipher[BUFFER_SIZE + AES_BLOCK_BYTES];
  uint8_t key[16] = {0};
  uint8_t iv[16] = {0};
  size_t m_len = 0;

  if (read_file(MESSAGE_FILE, message, BUFFER_SIZE, &m_len) == FAILURE)
    return FAILURE;
  if (read_key16(KEY_FILE, key) == FAILURE)
    return FAILURE;
  if (read_key16(INIT_FILE, iv) == FAILURE)
    return FAILURE;

  ctr_crypt_buffer(message, m_len, cipher, key, iv);

  if (write_file_bytes(CIPHER_FILE, cipher, m_len) == FAILURE)
    return FAILURE;
  printf("CTR encrypted %zu bytes.\n", m_len);
  return SUCCESS;
}

static int handle_ede_encrypt(void) {
  static uint8_t message[BUFFER_SIZE];
  static uint8_t cipher[BUFFER_SIZE + AES_BLOCK_BYTES];
  uint8_t k1[16], k2[16], k3[16];
  size_t m_len = 0;

  if (read_file(MESSAGE_FILE, message, BUFFER_SIZE, &m_len) == FAILURE)
    return FAILURE;
  if (read_key16(KEY1_FILE, k1) == FAILURE)
    return FAILURE;
  if (read_key16(KEY2_FILE, k2) == FAILURE)
    return FAILURE;
  if (read_key16(KEY3_FILE, k3) == FAILURE)
    return FAILURE;

  size_t out_len = 0;
  ede_encrypt_buffer(message, m_len, cipher, &out_len, k1, k2, k3);

  if (write_file_bytes(CIPHER_FILE, cipher, out_len) == FAILURE)
    return FAILURE;
  printf("EDE encrypted %zu bytes.\n", m_len);
  return SUCCESS;
}

static int handle_show(char *file_name, int power) {
  static const char *all_files[] = {MESSAGE_FILE, CIPHER_FILE, KEY_FILE,
                                    INIT_FILE,    KEY1_FILE,   KEY2_FILE,
                                    KEY3_FILE};
  int n_all = (int)(sizeof(all_files) / sizeof(all_files[0]));

  if (strcmp(file_name, SHOW_ALL) == 0) {
    for (int i = 0; i < n_all; i++)
      handle_show((char *)all_files[i], power);
    return SUCCESS;
  }

  if (!is_valid_file(file_name)) {
    printf("ERROR: unknown file '%s'.\n", file_name);
    return FAILURE;
  }

  static uint8_t buf[BUFFER_SIZE];
  size_t len = 0;
  if (read_file(file_name, buf, BUFFER_SIZE, &len) == FAILURE)
    return FAILURE;

  printf("\n====== %s ======\n", file_name);
  if (power == 16)
    print_file_in_hex((char *)buf, len);
  else
    print_file_in_symbol((char *)buf, len);
  printf("====== %s ======\n", file_name);
  return SUCCESS;
}

static int handle_write(char *file_name, char *text, int power) {
  if (!is_valid_file(file_name)) {
    printf("ERROR: unknown file '%s'.\n", file_name);
    return FAILURE;
  }

  static char bytes[BUFFER_SIZE];
  size_t bytes_len = 0;

  if (power == 16) {
    size_t text_len = strlen(text);
    if (convert_hex_to_symbol(text, text_len, bytes, &bytes_len) == FAILURE)
      return FAILURE;
  } else {
    bytes_len = strlen(text);
    memcpy(bytes, text, bytes_len);
  }

  return write_file_bytes(file_name, (uint8_t *)bytes, bytes_len);
}