#include "../lab1.h"

static int lfsr_encrypt(void);
static int lfsr_pick(int lfsr_num);
static int lfsr_show(char* file_name, int power);
static int lfsr_write(char* file_name, char* text, int power);
static int generate_lfsr_bit(unsigned char* initial_value);

int perform_task1_lfsr(char* buffer, size_t buffer_size) {
  (void)buffer_size;

  char file_name[BUFFER_SIZE], text[BUFFER_SIZE];
  int power, mode;

  if (parse_task1_lfsr_input(buffer, BUFFER_SIZE, &mode, file_name, &power,
                             text) == FAILURE)
    return FAILURE;

  if (mode == LFSR_ENCRYPT_OPTION_NUMBER) {
    if (lfsr_encrypt() == FAILURE) return FAILURE;
  } else if (mode == LFSR_PICK_OPTION_NUMBER) {
    if (lfsr_pick(power) == FAILURE) return FAILURE;
  } else if (mode == LFSR_SHOW_OPTION_NUMBER) {
    if (lfsr_show(file_name, power) == FAILURE) return FAILURE;
  } else if (mode == LFSR_WRITE_OPTION_NUMBER) {
    if (lfsr_write(file_name, text, power) == FAILURE) return FAILURE;
  }

  return SUCCESS;
}

void generate_key_with_lfsr(char* key_buffer, size_t key_len,
                            unsigned char initial_value) {
  for (size_t i = 0; i < key_len; i++) {
    unsigned char temp = 0;

    for (int j = 0; j < 8; j++) {
      int bit = generate_lfsr_bit(&initial_value);
      temp |= (bit << j);
    }

    key_buffer[i] = temp;
  }
}

static int generate_lfsr_bit(unsigned char* initial_value) {
  int all_polinomes[] = LFSR_ALL_LFSRS;
  int polinome = all_polinomes[current_lfsr_number];

  unsigned char new_initial = *initial_value;
  unsigned char temp = 0;
  for (int i = 0; i < LFSR_LENGTH; i++) {
    temp ^= (new_initial >> i) & ((polinome >> i) & 0b1);
  }
  *initial_value =
      (unsigned char)(temp << (LFSR_LENGTH - 1)) | (new_initial >> 1);

  return *initial_value & 0b1;
}

static int lfsr_encrypt(void) {
  char message[BUFFER_SIZE] = {0}, key[BUFFER_SIZE] = {0},
       cipher[BUFFER_SIZE] = {0};
  char initial_value[BUFFER_SIZE] = {0};

  int fd_m = open(MESSAGE_FILE, O_RDONLY);
  int fd_i = open(KEY_FILE, O_RDONLY);

  ssize_t bytes = read(fd_m, message, BUFFER_SIZE);
  read(fd_i, initial_value, BUFFER_SIZE);

  close(fd_m);
  close(fd_i);

  unsigned char actual_init_value = (unsigned char)initial_value[0];

  generate_key_with_lfsr(key, bytes, actual_init_value);
  printf("result key: %s\n", key);

  calculate_xor(message, bytes, key, bytes, cipher);

  int fd_c = open(CIPHER_FILE, O_WRONLY | O_TRUNC);
  write(fd_c, cipher, bytes);
  close(fd_c);

  return SUCCESS;
}

static int lfsr_pick(int lfsr_num) {
  if (lfsr_num == LFSR_1_NUMBER || lfsr_num == LFSR_2_NUMBER)
    current_lfsr_number = lfsr_num;
  else {
    printf("ERROR: lfsr_pick - wrong LFSR number.\n");
    return FAILURE;
  }

  return SUCCESS;
}

static int lfsr_show(char* file_name, int power) {
  int fd = open(file_name, O_RDONLY);

  char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);

  ssize_t bytes = read(fd, buffer, BUFFER_SIZE);

  printf("\n====== %s ======\n", file_name);
  if (power == 2)
    print_file_in_binary(buffer, bytes);
  else if (power == 16)
    print_file_in_hex(buffer, bytes);
  else if (power == 10)
    print_file_in_dec(buffer, bytes);
  else if (power == 0)
    print_file_in_symbol(buffer, bytes);
  printf("====== %s ======\n", file_name);

  close(fd);

  return SUCCESS;
}

static int lfsr_write(char* file_name, char* text, int power) {
  int fd = open(file_name, O_WRONLY | O_TRUNC);
  size_t len = strlen(text);

  int res = SUCCESS;
  char text_result[BUFFER_SIZE];
  size_t text_size = len;
  if (power == 2)
    res = convert_binary_to_symbol(text, len, text_result, &text_size);
  else if (power == 16)
    res = convert_hex_to_symbol(text, len, text_result, &text_size);
  else if (power == 10)
    res = convert_dec_to_symbol(text, len, text_result, &text_size);
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