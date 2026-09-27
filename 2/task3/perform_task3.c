#include "../lab2.h"

static int run_avalanche(int bit_pos);

int perform_task3(char* buffer, size_t buffer_size) {
  (void)buffer_size;

  char string_mode[BUFFER_SIZE];
  memset(string_mode, 0, sizeof(string_mode));
  int bit_pos = 0;

  int n = sscanf(buffer, "%s %d", string_mode, &bit_pos);

  if (n < 1) {
    printf("ERROR: perform_task3 - cannot parse input.\n");
    return FAILURE;
  }

  if (strcmp(string_mode, BACK_EXIT_OPTION) == 0) return SUCCESS;

  if (strcmp(string_mode, EXAMINE_AVALANCHE_OPTION) == 0) {
    if (n == 1) {
      printf("Note: no bit position given, using default 0.\n");
      bit_pos = 0;
    }
    if (bit_pos < 0 || bit_pos > 63) {
      printf("ERROR: bit position must be in [0, 63] (block is 64 bits).\n");
      return FAILURE;
    }
    return run_avalanche(bit_pos);
  }

  printf("ERROR: mode is not found.\n");
  return FAILURE;
}

static int count_diff_bits(const unsigned char* a, const unsigned char* b,
                           size_t bytes) {
  int cnt = 0;
  for (size_t i = 0; i < bytes; i++) {
    unsigned char x = a[i] ^ b[i];
    while (x) {
      cnt += x & 1;
      x >>= 1;
    }
  }
  return cnt;
}

static void write_csv(const char* fname, const unsigned char* rounds_orig,
                      const unsigned char* rounds_mod) {
  int fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd == -1) {
    perror("open csv");
    return;
  }
  char line[64];
  int len = snprintf(line, sizeof(line), "round,bits_changed\n");
  write(fd, line, len);

  for (int r = 0; r < FEISTEL_ROUNDS; r++) {
    const unsigned char* a = rounds_orig + r * FEISTEL_BLOCK_BYTES;
    const unsigned char* b = rounds_mod + r * FEISTEL_BLOCK_BYTES;
    int d = count_diff_bits(a, b, FEISTEL_BLOCK_BYTES);
    len = snprintf(line, sizeof(line), "%d,%d\n", r + 1, d);
    write(fd, line, len);
  }
  close(fd);
}

static void run_case(const unsigned char* msg, const unsigned char* key,
                     int flip_in_key, int bit_pos, const feistel_settings* s,
                     const char* fname) {
  unsigned char msg2[FEISTEL_BLOCK_BYTES];
  unsigned char key2[FEISTEL_KEY_BYTES];
  memcpy(msg2, msg, FEISTEL_BLOCK_BYTES);
  memcpy(key2, key, FEISTEL_KEY_BYTES);

  if (flip_in_key)
    key2[bit_pos / 8] ^= (1 << (7 - (bit_pos % 8)));
  else
    msg2[bit_pos / 8] ^= (1 << (7 - (bit_pos % 8)));

  unsigned char out1[FEISTEL_BLOCK_BYTES], out2[FEISTEL_BLOCK_BYTES];
  unsigned char r1[FEISTEL_ROUNDS][FEISTEL_BLOCK_BYTES];
  unsigned char r2[FEISTEL_ROUNDS][FEISTEL_BLOCK_BYTES];

  feistel_encrypt_block_track(msg, out1, key, s, r1);
  feistel_encrypt_block_track(msg2, out2, key2, s, r2);

  write_csv(fname, (unsigned char*)r1, (unsigned char*)r2);
  printf("  wrote %s\n", fname);
}

static int run_avalanche(int bit_pos) {
  unsigned char message[BUFFER_SIZE] = {0};
  unsigned char key[FEISTEL_KEY_BYTES] = {0};

  int fd_m = open(MESSAGE_FILE, O_RDONLY);
  int fd_k = open(KEY_FILE, O_RDONLY);
  if (fd_m == -1 || fd_k == -1) {
    perror("open");
    if (fd_m != -1) close(fd_m);
    if (fd_k != -1) close(fd_k);
    return FAILURE;
  }
  read(fd_m, message, BUFFER_SIZE);
  read(fd_k, key, FEISTEL_KEY_BYTES);
  close(fd_m);
  close(fd_k);

  printf("Avalanche research for bit position %d:\n", bit_pos);

  for (int km = 1; km <= 2; km++) {
    for (int fn = 1; fn <= 2; fn++) {
      feistel_settings s = {km, fn};

      char fname[96];
      snprintf(fname, sizeof(fname), "avalanche_km%d_fn%d_bit%d_ptext.csv", km,
               fn, bit_pos);
      run_case(message, key, 0, bit_pos, &s, fname);

      snprintf(fname, sizeof(fname), "avalanche_km%d_fn%d_bit%d_key.csv", km,
               fn, bit_pos);
      run_case(message, key, 1, bit_pos, &s, fname);
    }
  }

  printf("8 CSV files written for bit %d.\n", bit_pos);
  return SUCCESS;
}