#include <math.h>

#include "../lab1.h"

static int find_period(void);
static int check_chi_2(void);
static int check_properties(void);

static void check_balance(char* buffer, size_t buffer_size);
static void check_cycles(char* buffer, size_t buffer_size);
static void check_correlation(char* buffer, size_t buffer_size);

int perform_task3(char* buffer, size_t buffer_size) {
  (void)buffer_size;

  int mode = BACK_EXIT_OPTION_NUMBER;
  if (get_mode_from_input(buffer, &mode) == FAILURE) return FAILURE;

  if (mode == BACK_EXIT_OPTION_NUMBER) return SUCCESS;

  int res = SUCCESS;
  if (mode == EXAMINE_FIND_PERIOD_OPTION_NUMBER) {
    res = find_period();
  } else if (mode == EXAMINE_CHECK_CHI_2_OPTION_NUMBER) {
    res = check_chi_2();
  } else if (mode == EXAMINE_CHECK_PROPERTIES_OPTION_NUMBER) {
    res = check_properties();
  } else {
    printf("ERROR: mode is not found.\n");
    return FAILURE;
  }

  return res;
}

static int find_period(void) {
  char initial_value[BUFFER_SIZE] = {0};

  int fd_i = open(KEY_FILE, O_RDONLY);
  read(fd_i, initial_value, BUFFER_SIZE);
  close(fd_i);

  unsigned char actual_init_value = (unsigned char)initial_value[0];

  size_t rows = LFSR_LENGTH;
  size_t row_len = LFSR_LENGTH + 1;
  char* numbers = (char*)calloc(rows * row_len, sizeof(char));

  size_t row_i = 0;
  for (int j = 0; j < LFSR_LENGTH; j++) {
    numbers[row_i * row_len + j] =
        ((((unsigned int)actual_init_value >> (LFSR_LENGTH - 1 - j))) & 0b1) +
        '0';
  }
  row_i++;

  int period_is_found = FALSE;
  while (!period_is_found) {
    if (rows <= row_i) {
      rows *= 2;
      char* temp = (char*)realloc(numbers, rows * row_len);

      numbers = temp;
    }

    generate_lfsr_bit(&actual_init_value);
    for (int j = 0; j < LFSR_LENGTH; j++) {
      numbers[row_i * row_len + j] =
          ((((unsigned int)actual_init_value >> (LFSR_LENGTH - 1 - j))) & 0b1) +
          '0';
    }
    numbers[row_i * row_len + LFSR_LENGTH] = '\0';

    for (size_t k = 0; k < row_i; k++) {
      if (strcmp(numbers + k * row_len, numbers + row_i * row_len) == 0) {
        printf("The period is found - %ld\n", row_i - k);
        printf("All calculated values:\n");
        for (size_t a = 0; a < row_i + 1; a++) {
          printf("%3ld: %s%s\n", a, numbers + a * row_len,
                 (a == row_i || a == k) ? " < period" : "");
        }
        period_is_found = TRUE;
        break;
      }
    }

    row_i++;
  }

  free(numbers);

  return SUCCESS;
}

static int check_chi_2(void) {
  char initial_value[BUFFER_SIZE] = {0};
  int fd_i = open(KEY_FILE, O_RDONLY);
  read(fd_i, initial_value, BUFFER_SIZE);
  close(fd_i);

  unsigned char actual_init_value = (unsigned char)initial_value[0];

  int number_of_bits = 1000000;
  int number_of_1 = 0;
  int number_of_0 = 0;
  for (int i = 0; i < number_of_bits; i++) {
    int bit = generate_lfsr_bit(&actual_init_value);
    if (bit == 1)
      number_of_1++;
    else
      number_of_0++;
  }

  double expected = number_of_bits / 2.0;
  double chi_2 = 0;
  chi_2 += (number_of_1 - expected) * (number_of_1 - expected) / expected;
  chi_2 += (number_of_0 - expected) * (number_of_0 - expected) / expected;

  double stat = 3.841;
  printf("(a = 0.05): %.3lf %s %.3lf - %s\n", chi_2,
         chi_2 < stat ? "<" : ">=", stat, chi_2 < stat ? "good" : "bad!");

  return SUCCESS;
}

static int check_properties(void) {
  char initial_value[BUFFER_SIZE] = {0};
  int fd_i = open(KEY_FILE, O_RDONLY);
  read(fd_i, initial_value, BUFFER_SIZE);
  close(fd_i);

  unsigned char actual_init_value = (unsigned char)initial_value[0];

  size_t number_of_bytes = BUFFER_SIZE * 256;
  char* bytes = (char*)calloc(number_of_bytes, sizeof(char));
  for (size_t i = 0; i < number_of_bytes; i++) {
    for (int j = 0; j < 8; j++) {
      int bit = generate_lfsr_bit(&actual_init_value);
      bytes[i] |= bit << (8 - 1 - j);
    }
  }

  check_balance(bytes, number_of_bytes);
  printf("\n");

  check_cycles(bytes, number_of_bytes);
  printf("\n");

  check_correlation(bytes, number_of_bytes);

  free(bytes);

  return SUCCESS;
}

static void check_balance(char* buffer, size_t buffer_size) {
  size_t number_of_1 = 0;
  size_t number_of_0 = 0;

  int byte_size = sizeof(buffer[0]) * 8;

  for (size_t i = 0; i < buffer_size; i++) {
    for (int j = 0; j < byte_size; j++) {
      int bit = (buffer[i] >> (byte_size - 1 - j)) & 0b1;
      if (bit)
        number_of_1++;
      else
        number_of_0++;
    }
  }
  double diff = (number_of_0 > number_of_1 ? number_of_0 - number_of_1
                                           : number_of_1 - number_of_0) /
                (double)(buffer_size * byte_size);

  double percents = 3.0;
  printf("balance: (0) %ld vs %ld (1) - %.3lf%% %s %.3lf%% - %s\n", number_of_0,
         number_of_1, diff * 100.0,
         diff * 100.0 < percents ? "<" : ">=", percents,
         diff * 100.0 < percents ? "good" : "bad!");
}

static void check_cycles(char* buffer, size_t buffer_size) {
  size_t number_of_lens = 5;
  size_t* first_cycles = (size_t*)calloc(number_of_lens, sizeof(size_t));
  int current_bit = 0;
  size_t current_len = 0;

  int byte_size = sizeof(buffer[0]) * 8;

  for (size_t i = 0; i < buffer_size; i++) {
    for (int j = 0; j < byte_size; j++) {
      int bit = (buffer[i] >> (byte_size - 1 - j)) & 0b1;
      if (bit != current_bit) {
        if (current_len >= 1 && current_len <= number_of_lens)
          first_cycles[current_len - 1]++;
        current_bit = bit;
        current_len = 1;
      } else
        current_len++;
    }
  }

  printf("cycles:\n");
  for (size_t i = 0; i < number_of_lens; i++) {
    printf("%ld: %ld, expected: %.1lf\n", i + 1, first_cycles[i],
           pow(2.0, -(i + 2.0)) * (double)buffer_size * (double)byte_size);
  }
}

static void check_correlation(char* buffer, size_t buffer_size) {
  int byte_size = sizeof(buffer[0]) * 8;
  size_t number_of_bits = buffer_size * byte_size;
  double percents = 3.0;
  int is_good = TRUE;

  size_t max_shift = 10;

  for (size_t shift = 1; shift <= max_shift; shift++) {
    size_t number_of_matches = 0;
    size_t number_of_mismatches = 0;

    for (size_t i = 0; i < number_of_bits; i++) {
      size_t j = (i + shift) % number_of_bits;

      int bit_i =
          (buffer[i / byte_size] >> (byte_size - 1 - (i % byte_size))) & 0b1;
      int bit_j =
          (buffer[j / byte_size] >> (byte_size - 1 - (j % byte_size))) & 0b1;

      if (bit_i == bit_j)
        number_of_matches++;
      else
        number_of_mismatches++;
    }

    double diff = (number_of_matches > number_of_mismatches
                       ? number_of_matches - number_of_mismatches
                       : number_of_mismatches - number_of_matches) /
                  (double)number_of_bits;

    printf("shift %ld: %.3lf%% %s %.3lf%% - %s\n", shift, diff * 100.0,
           diff * 100.0 < percents ? "<" : ">=", percents,
           diff * 100.0 < percents ? "good" : "bad!");
    if (diff * 100.0 >= percents) is_good = FALSE;
  }

  printf("correlation: %s %.3lf%%\n", is_good ? "good" : "bad!", percents);
}
