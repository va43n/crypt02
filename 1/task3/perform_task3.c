#include "../lab1.h"

static int find_period(void);
static int check_chi_2(void);
static int check_properties(void);

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

static int check_chi_2(void) { return SUCCESS; }

static int check_properties(void) { return SUCCESS; }