#include "../lab1.h"

static int decrypt_with_xor(void);
static int decrypt_with_lfsr(void);

int perform_task2(char *buffer, size_t buffer_size) {
  (void)buffer_size;

  int mode = BACK_EXIT_OPTION_NUMBER;
  if (get_mode_from_input(buffer, &mode) == FAILURE) return FAILURE;

  if (mode == BACK_EXIT_OPTION_NUMBER) return SUCCESS;

  int res = SUCCESS;
  if (mode == XOR_ENCRYPT_OPTION_NUMBER) {
    res = decrypt_with_xor();
  } else if (mode == LFSR_ENCRYPT_OPTION_NUMBER) {
    res = decrypt_with_lfsr();
  } else {
    printf("ERROR: mode is not found.\n");
    return FAILURE;
  }

  return res;
}

static int decrypt_with_xor(void) {
  char message[BUFFER_SIZE], key[BUFFER_SIZE], cipher[BUFFER_SIZE];

  int fd_c = open(CIPHER_FILE, O_RDONLY);
  int fd_k = open(KEY_FILE, O_RDONLY);

  ssize_t c_bytes = read(fd_c, cipher, BUFFER_SIZE);
  ssize_t k_bytes = read(fd_k, key, BUFFER_SIZE);

  close(fd_c);
  close(fd_k);

  calculate_xor(cipher, c_bytes, key, k_bytes, message);

  int fd_m = open(MESSAGE_FILE, O_WRONLY | O_TRUNC);
  write(fd_m, message, c_bytes);
  close(fd_m);

  return SUCCESS;
}

static int decrypt_with_lfsr(void) {
  char message[BUFFER_SIZE] = {0}, key[BUFFER_SIZE] = {0},
       cipher[BUFFER_SIZE] = {0};
  char initial_value[BUFFER_SIZE] = {0};

  int fd_c = open(CIPHER_FILE, O_RDONLY);
  int fd_i = open(KEY_FILE, O_RDONLY);

  ssize_t bytes = read(fd_c, cipher, BUFFER_SIZE);
  read(fd_i, initial_value, BUFFER_SIZE);

  close(fd_c);
  close(fd_i);

  unsigned char actual_init_value = (unsigned char)initial_value[0];

  generate_key_with_lfsr(key, bytes, actual_init_value);
  printf("result key: ");
  print_file_in_hex(key, bytes);

  calculate_xor(cipher, bytes, key, bytes, message);

  int fd_m = open(MESSAGE_FILE, O_WRONLY | O_TRUNC);
  write(fd_m, message, bytes);
  close(fd_m);

  return SUCCESS;
}
