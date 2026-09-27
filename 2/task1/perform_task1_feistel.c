#include "../lab2.h"

static int feistel_encrypt(void);
static int feistel_show(char* file_name, int power);
static int feistel_write(char* file_name, char* text, int power);
static int feistel_pick_key_method(int method);
static int feistel_pick_func(int func);

int perform_task1_feistel(char* buffer, size_t buffer_size) {
  (void)buffer_size;

  char file_name[BUFFER_SIZE], text[BUFFER_SIZE];
  int power, mode;

  if (parse_task1_feistel_input(buffer, BUFFER_SIZE, &mode, file_name, &power,
                                text) == FAILURE)
    return FAILURE;

  switch (mode) {
    case FEISTEL_ENCRYPT_OPTION_NUMBER:
      return feistel_encrypt();
    case FEISTEL_SHOW_OPTION_NUMBER:
      return feistel_show(file_name, power);
    case FEISTEL_WRITE_OPTION_NUMBER:
      return feistel_write(file_name, text, power);
    case FEISTEL_PICK_KEY_METHOD_OPTION_NUMBER:
      return feistel_pick_key_method(power);
    case FEISTEL_PICK_FUNC_OPTION_NUMBER:
      return feistel_pick_func(power);
    default:
      return FAILURE;
  }
}

static int feistel_encrypt(void) {
  unsigned char message[BUFFER_SIZE] = {0};
  unsigned char key[FEISTEL_KEY_BYTES] = {0};
  unsigned char cipher[BUFFER_SIZE] = {0};

  int fd_m = open(MESSAGE_FILE, O_RDONLY);
  int fd_k = open(KEY_FILE, O_RDONLY);
  if (fd_m == -1 || fd_k == -1) {
    perror("open");
    if (fd_m != -1) close(fd_m);
    if (fd_k != -1) close(fd_k);
    return FAILURE;
  }

  ssize_t m_bytes = read(fd_m, message, BUFFER_SIZE);
  ssize_t k_bytes = read(fd_k, key, FEISTEL_KEY_BYTES);
  close(fd_m);
  close(fd_k);

  if (m_bytes < 0) m_bytes = 0;
  if (k_bytes < FEISTEL_KEY_BYTES) {
    for (int i = (int)(k_bytes > 0 ? k_bytes : 0); i < FEISTEL_KEY_BYTES; i++)
      key[i] = 0;
  }

  feistel_settings s = {current_key_method, current_func_type};

  size_t out_len = 0;
  feistel_encrypt_buffer(message, (size_t)m_bytes, cipher, &out_len, key, &s);

  int fd_c = open(CIPHER_FILE, O_WRONLY | O_TRUNC);
  if (fd_c == -1) {
    perror("open cipher");
    return FAILURE;
  }
  write(fd_c, cipher, out_len);
  close(fd_c);

  printf("Encrypted %zd bytes -> %zu bytes.\n", m_bytes, out_len);
  return SUCCESS;
}

static int feistel_show(char* file_name, int power) {
  char* file_names[] = {MESSAGE_FILE, CIPHER_FILE, KEY_FILE};
  int file_names_number = 3;

  for (int i = 0; i < file_names_number; i++) {
    if (strcmp(file_name, FEISTEL_SHOW_ALL) != 0 &&
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
    if (power == 2)
      print_file_in_binary(buffer, len);
    else if (power == 16)
      print_file_in_hex(buffer, len);
    else if (power == 0)
      print_file_in_symbol(buffer, len);
    printf("====== %s ======\n", file_names[i]);
  }

  return SUCCESS;
}

static int feistel_write(char* file_name, char* text, int power) {
  int fd = open(file_name, O_WRONLY | O_TRUNC);
  if (fd == -1) {
    perror("open");
    return FAILURE;
  }

  size_t len = strlen(text);
  int res = SUCCESS;
  char text_result[BUFFER_SIZE];
  size_t text_size = len;

  if (power == 2)
    res = convert_binary_to_symbol(text, len, text_result, &text_size);
  else if (power == 16)
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

static int feistel_pick_key_method(int method) {
  if (method != FEISTEL_KEY_METHOD_A && method != FEISTEL_KEY_METHOD_B) {
    printf("ERROR: key method must be 1 or 2.\n");
    return FAILURE;
  }
  current_key_method = method;
  printf("Current key method: %d\n", current_key_method);
  return SUCCESS;
}

static int feistel_pick_func(int func) {
  if (func != FEISTEL_FUNC_A && func != FEISTEL_FUNC_B) {
    printf("ERROR: func must be 1 or 2.\n");
    return FAILURE;
  }
  current_func_type = func;
  printf("Current func type: %d\n", current_func_type);
  return SUCCESS;
}