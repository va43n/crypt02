#include "../lab1.h"

static int xor_encrypt(void);
static int xor_gen(char* file_name);
static int xor_show(char* file_name, int power);
static int xor_write(char* file_name, char* text, int power);

int perform_task1(char* buffer, size_t buffer_size) {
  char file_name[BUFFER_SIZE], text[BUFFER_SIZE];
  int power, mode;

  if (parse_task1_input(buffer, BUFFER_SIZE, &mode, file_name, &power, text) ==
      FAILURE)
    return FAILURE;

  memset(buffer, 0, buffer_size);
  if (mode == XOR_ENCRYPT_OPTION_NUMBER) {
    if (xor_encrypt() == FAILURE) return FAILURE;
  } else if (mode == XOR_GEN_OPTION_NUMBER) {
    if (xor_gen(file_name) == FAILURE) return FAILURE;
  } else if (mode == XOR_SHOW_OPTION_NUMBER) {
    if (xor_show(file_name, power) == FAILURE) return FAILURE;
  } else if (mode == XOR_WRITE_OPTION_NUMBER) {
    if (xor_write(file_name, text, power) == FAILURE) return FAILURE;
  }

  return SUCCESS;
}

static int xor_encrypt(void) {
  char message[BUFFER_SIZE], key[BUFFER_SIZE], cipher[BUFFER_SIZE];

  int fd_m = open(MESSAGE_FILE, O_RDONLY);
  int fd_k = open(KEY_FILE, O_RDONLY);

  read(fd_m, message, BUFFER_SIZE);
  read(fd_k, key, BUFFER_SIZE);

  close(fd_m);
  close(fd_k);

  calculate_xor(message, key, cipher);
  size_t c_len = strlen(cipher);

  int fd_c = open(CIPHER_FILE, O_WRONLY | O_TRUNC);
  write(fd_c, cipher, c_len);
  close(fd_c);

  return SUCCESS;
}

static int xor_gen(char* file_name) {
  int fd = open(file_name, O_WRONLY | O_TRUNC);

  char gen_text[XOR_GEN_SIZE];
  for (int i = 0; i < XOR_GEN_SIZE; i++) {
    gen_text[i] = (unsigned char)(rand() % 0xFE + 1);
  }
  write(fd, gen_text, XOR_GEN_SIZE);

  close(fd);

  return SUCCESS;
}

static int xor_show(char* file_name, int power) {
  char* file_names[] = {MESSAGE_FILE, CIPHER_FILE, KEY_FILE};
  int file_names_number = 3;

  for (int i = 0; i < file_names_number; i++) {
    if (strcmp(file_name, XOR_SHOW_ALL) != 0 &&
        strcmp(file_name, file_names[i]) != 0)
      continue;

    int fd = open(file_names[i], O_RDONLY);

    char buffer[BUFFER_SIZE];

    read(fd, buffer, BUFFER_SIZE);
    size_t len = strlen(buffer);

    printf("\n====== %s ======\n", file_names[i]);
    if (power == 2)
      print_file_in_binary(buffer, len);
    else if (power == 16)
      print_file_in_hex(buffer, len);
    else if (power == 0)
      print_file_in_symbol(buffer, len);
    printf("====== %s ======\n", file_names[i]);

    close(fd);
  }

  return SUCCESS;
}

static int xor_write(char* file_name, char* text, int power) {
  int fd = open(file_name, O_WRONLY | O_TRUNC);
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