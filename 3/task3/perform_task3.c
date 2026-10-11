#include <math.h>

#include "../lab3.h"

static int run_avalanche(int bit_pos);
static int run_criteria(void);

static int count_diff_bits(const uint8_t* a, const uint8_t* b, size_t bytes);
static void flip_bit(uint8_t* buf, int bit_pos);
static int load_first_block(uint8_t block[AES_BLOCK_BYTES]);
static int load_key(uint8_t key[AES_KEY_BYTES]);
static void write_avalanche_csv(
    const char* fname, uint8_t states[AES_ROUNDS + 1][AES_BLOCK_BYTES],
    uint8_t states_mod[AES_ROUNDS + 1][AES_BLOCK_BYTES]);
static void save_matrix(const char* fname, uint32_t** m, int rows, int cols);
static void free_matrices(uint32_t** a_text, uint32_t** b_text,
                          uint32_t** a_key, uint32_t** b_key, int n);
static int run_criteria_gost(void);

int perform_task3(char* buffer, size_t buffer_size) {
  (void)buffer_size;

  char string_mode[BUFFER_SIZE];
  int bit_pos = 0;
  memset(string_mode, 0, sizeof(string_mode));

  int n = sscanf(buffer, "%s %d", string_mode, &bit_pos);
  if (n < 1) {
    printf("ERROR: cannot parse input.\n");
    return FAILURE;
  }

  if (strcmp(string_mode, BACK_EXIT_OPTION) == 0) return SUCCESS;

  if (strcmp(string_mode, AES_AVALANCHE_OPTION) == 0) {
    if (n < 2) {
      printf("Note: no bit position given, using default 0.\n");
      bit_pos = 0;
    }
    if (bit_pos < 0 || bit_pos > 127) {
      printf("ERROR: bit position must be in [0, 127].\n");
      return FAILURE;
    }
    return run_avalanche(bit_pos);
  }

  if (strcmp(string_mode, AES_CRITERIA_OPTION) == 0) {
    return run_criteria();
  }
  if (strcmp(string_mode, GOST_CRITERIA_OPTION) == 0) {
    return run_criteria_gost();
  }

  printf("ERROR: mode is not found.\n");
  return FAILURE;
}

static int count_diff_bits(const uint8_t* a, const uint8_t* b, size_t bytes) {
  int cnt = 0;
  for (size_t i = 0; i < bytes; i++) {
    uint8_t x = (uint8_t)(a[i] ^ b[i]);
    while (x) {
      cnt += x & 1;
      x >>= 1;
    }
  }
  return cnt;
}

static void flip_bit(uint8_t* buf, int bit_pos) {
  buf[bit_pos / 8] ^= (uint8_t)(1 << (7 - (bit_pos % 8)));
}

static int load_first_block(uint8_t block[AES_BLOCK_BYTES]) {
  int fd = open(MESSAGE_FILE, O_RDONLY);
  if (fd == -1) {
    perror("open message");
    return FAILURE;
  }
  memset(block, 0, AES_BLOCK_BYTES);
  read(fd, block, AES_BLOCK_BYTES);
  close(fd);
  return SUCCESS;
}

static int load_key(uint8_t key[AES_KEY_BYTES]) {
  int fd = open(KEY_FILE, O_RDONLY);
  if (fd == -1) {
    perror("open key");
    return FAILURE;
  }
  memset(key, 0, AES_KEY_BYTES);
  read(fd, key, AES_KEY_BYTES);
  close(fd);
  return SUCCESS;
}

static void write_avalanche_csv(
    const char* fname, uint8_t states[AES_ROUNDS + 1][AES_BLOCK_BYTES],
    uint8_t states_mod[AES_ROUNDS + 1][AES_BLOCK_BYTES]) {
  int fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd == -1) {
    perror("open csv");
    return;
  }

  char line[64];
  int len = snprintf(line, sizeof(line), "round,bits_changed\n");
  write(fd, line, (size_t)len);

  for (int r = 0; r <= AES_ROUNDS; r++) {
    int d = count_diff_bits(states[r], states_mod[r], AES_BLOCK_BYTES);
    len = snprintf(line, sizeof(line), "%d,%d\n", r, d);
    write(fd, line, (size_t)len);
  }
  close(fd);
}

static void save_matrix(const char* fname, uint32_t** m, int rows, int cols) {
  int fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd == -1) {
    perror("open csv");
    return;
  }

  char buf[16];
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int len = snprintf(buf, sizeof(buf), "%u", (unsigned)m[i][j]);
      write(fd, buf, (size_t)len);
      if (j < cols - 1) write(fd, ",", 1);
    }
    write(fd, "\n", 1);
  }
  close(fd);
}

static void free_matrices(uint32_t** a_text, uint32_t** b_text,
                          uint32_t** a_key, uint32_t** b_key, int n) {
  if (a_text) {
    for (int i = 0; i < n; i++) free(a_text[i]);
    free(a_text);
  }
  if (b_text) {
    for (int i = 0; i < n; i++) free(b_text[i]);
    free(b_text);
  }
  if (a_key) {
    for (int i = 0; i < n; i++) free(a_key[i]);
    free(a_key);
  }
  if (b_key) {
    for (int i = 0; i < n; i++) free(b_key[i]);
    free(b_key);
  }
}

static int run_avalanche(int bit_pos) {
  uint8_t block[AES_BLOCK_BYTES];
  uint8_t key[AES_KEY_BYTES];

  if (load_first_block(block) == FAILURE) return FAILURE;
  if (load_key(key) == FAILURE) return FAILURE;

  uint8_t out1[AES_BLOCK_BYTES], out2[AES_BLOCK_BYTES];
  uint8_t states1[AES_ROUNDS + 1][AES_BLOCK_BYTES];
  uint8_t states2[AES_ROUNDS + 1][AES_BLOCK_BYTES];

  uint8_t block_mod[AES_BLOCK_BYTES];
  memcpy(block_mod, block, AES_BLOCK_BYTES);
  flip_bit(block_mod, bit_pos);

  aes_encrypt_block_track(block, out1, key, states1);
  aes_encrypt_block_track(block_mod, out2, key, states2);

  {
    char fname[96];
    snprintf(fname, sizeof(fname), "avalanche_text_bit%d.csv", bit_pos);
    write_avalanche_csv(fname, states1, states2);
    printf("  wrote %s\n", fname);
  }

  uint8_t key_mod[AES_KEY_BYTES];
  memcpy(key_mod, key, AES_KEY_BYTES);
  flip_bit(key_mod, bit_pos);

  aes_encrypt_block_track(block, out1, key, states1);
  aes_encrypt_block_track(block, out2, key_mod, states2);

  {
    char fname[96];
    snprintf(fname, sizeof(fname), "avalanche_key_bit%d.csv", bit_pos);
    write_avalanche_csv(fname, states1, states2);
    printf("  wrote %s\n", fname);
  }

  return SUCCESS;
}

static int run_criteria(void) {
  const int n_bits = 128;
  const int m_bits = 128;

  uint8_t* data = NULL;
  uint8_t* padded = NULL;
  uint8_t* Y_all = NULL;
  uint32_t** a_text = NULL;
  uint32_t** b_text = NULL;
  uint32_t** a_key = NULL;
  uint32_t** b_key = NULL;

  data = malloc(BUFFER_SIZE);
  if (!data) {
    perror("malloc");
    return FAILURE;
  }

  int fd = open(MESSAGE_FILE, O_RDONLY);
  if (fd == -1) {
    perror("open message");
    free(data);
    return FAILURE;
  }
  ssize_t n = read(fd, data, BUFFER_SIZE);
  close(fd);
  if (n < 0) n = 0;
  size_t data_len = (size_t)n;

  size_t N = (data_len + AES_BLOCK_BYTES - 1) / AES_BLOCK_BYTES;
  if (N == 0) N = 1;
  size_t padded_len = N * AES_BLOCK_BYTES;

  padded = calloc(padded_len, 1);
  if (!padded) {
    perror("calloc");
    free(data);
    return FAILURE;
  }
  memcpy(padded, data, data_len);
  free(data);
  data = NULL;

  uint8_t key[AES_KEY_BYTES];
  if (load_key(key) == FAILURE) {
    free(padded);
    return FAILURE;
  }

  Y_all = malloc(N * AES_BLOCK_BYTES);
  if (!Y_all) {
    perror("malloc");
    free(padded);
    return FAILURE;
  }

  for (size_t b = 0; b < N; b++)
    aes_encrypt_block(padded + b * AES_BLOCK_BYTES, Y_all + b * AES_BLOCK_BYTES,
                      key);

  a_text = calloc((size_t)n_bits, sizeof(uint32_t*));
  b_text = calloc((size_t)n_bits, sizeof(uint32_t*));
  a_key = calloc((size_t)n_bits, sizeof(uint32_t*));
  b_key = calloc((size_t)n_bits, sizeof(uint32_t*));

  if (!a_text || !b_text || !a_key || !b_key) {
    perror("calloc");
    free(padded);
    free(Y_all);
    free_matrices(a_text, b_text, a_key, b_key, n_bits);
    return FAILURE;
  }

  int alloc_ok = 1;
  for (int i = 0; i < n_bits; i++) {
    a_text[i] = calloc((size_t)m_bits, sizeof(uint32_t));
    b_text[i] = calloc((size_t)m_bits + 1, sizeof(uint32_t));
    a_key[i] = calloc((size_t)m_bits, sizeof(uint32_t));
    b_key[i] = calloc((size_t)m_bits + 1, sizeof(uint32_t));
    if (!a_text[i] || !b_text[i] || !a_key[i] || !b_key[i]) {
      alloc_ok = 0;
      break;
    }
  }
  if (!alloc_ok) {
    perror("calloc");
    free(padded);
    free(Y_all);
    free_matrices(a_text, b_text, a_key, b_key, n_bits);
    return FAILURE;
  }

  for (int i = 0; i < n_bits; i++) {
    for (size_t b = 0; b < N; b++) {
      uint8_t X[AES_BLOCK_BYTES];
      memcpy(X, padded + b * AES_BLOCK_BYTES, AES_BLOCK_BYTES);
      flip_bit(X, i);

      uint8_t Y[AES_BLOCK_BYTES];
      aes_encrypt_block(X, Y, key);

      uint8_t* Yb = Y_all + b * AES_BLOCK_BYTES;
      int w = count_diff_bits(Y, Yb, AES_BLOCK_BYTES);
      b_text[i][w]++;

      for (int j = 0; j < m_bits; j++) {
        uint8_t x = (uint8_t)(Y[j / 8] ^ Yb[j / 8]);
        if (x & (1 << (7 - (j % 8)))) a_text[i][j]++;
      }
    }
  }

  for (int i = 0; i < n_bits; i++) {
    uint8_t key_mod[AES_KEY_BYTES];
    memcpy(key_mod, key, AES_KEY_BYTES);
    flip_bit(key_mod, i);

    for (size_t b = 0; b < N; b++) {
      uint8_t Y[AES_BLOCK_BYTES];
      aes_encrypt_block(padded + b * AES_BLOCK_BYTES, Y, key_mod);

      uint8_t* Yb = Y_all + b * AES_BLOCK_BYTES;
      int w = count_diff_bits(Y, Yb, AES_BLOCK_BYTES);
      b_key[i][w]++;

      for (int j = 0; j < m_bits; j++) {
        uint8_t x = (uint8_t)(Y[j / 8] ^ Yb[j / 8]);
        if (x & (1 << (7 - (j % 8)))) a_key[i][j]++;
      }
    }
  }

  double s1_text = 0, s1_key = 0;
  for (int i = 0; i < n_bits; i++) {
    double sj_t = 0, sj_k = 0;
    for (int j = 0; j <= m_bits; j++) {
      sj_t += (double)j * b_text[i][j];
      sj_k += (double)j * b_key[i][j];
    }
    s1_text += sj_t / (double)N;
    s1_key += sj_k / (double)N;
  }
  double d1_text = s1_text / n_bits;
  double d1_key = s1_key / n_bits;

  long zero_a_text = 0, zero_a_key = 0;
  for (int i = 0; i < n_bits; i++) {
    for (int j = 0; j < m_bits; j++) {
      if (a_text[i][j] == 0) zero_a_text++;
      if (a_key[i][j] == 0) zero_a_key++;
    }
  }
  double d2_text = 1.0 - (double)zero_a_text / ((double)n_bits * m_bits);
  double d2_key = 1.0 - (double)zero_a_key / ((double)n_bits * m_bits);

  double s3_text = 0, s3_key = 0;
  for (int i = 0; i < n_bits; i++) {
    double inner_t = 0, inner_k = 0;
    for (int j = 0; j <= m_bits; j++) {
      inner_t += 2.0 * j * b_text[i][j];
      inner_k += 2.0 * j * b_key[i][j];
    }
    s3_text += fabs(inner_t / (double)N - m_bits);
    s3_key += fabs(inner_k / (double)N - m_bits);
  }
  double d3_text = 1.0 - s3_text / ((double)n_bits * m_bits);
  double d3_key = 1.0 - s3_key / ((double)n_bits * m_bits);

  double s4_text = 0, s4_key = 0;
  for (int i = 0; i < n_bits; i++) {
    for (int j = 0; j < m_bits; j++) {
      s4_text += fabs(2.0 * a_text[i][j] / (double)N - 1.0);
      s4_key += fabs(2.0 * a_key[i][j] / (double)N - 1.0);
    }
  }
  double d4_text = 1.0 - s4_text / ((double)n_bits * m_bits);
  double d4_key = 1.0 - s4_key / ((double)n_bits * m_bits);

  printf("\n=== Criteria (N = %zu blocks) ===\n", N);
  printf("Diffusion (text):  d1=%.4f  d2=%.4f  d3=%.4f  d4=%.4f\n", d1_text,
         d2_text, d3_text, d4_text);
  printf("Confusion (key):   d1=%.4f  d2=%.4f  d3=%.4f  d4=%.4f\n", d1_key,
         d2_key, d3_key, d4_key);

  save_matrix("criteria_a_text.csv", a_text, n_bits, m_bits);
  save_matrix("criteria_b_text.csv", b_text, n_bits, m_bits + 1);
  save_matrix("criteria_a_key.csv", a_key, n_bits, m_bits);
  save_matrix("criteria_b_key.csv", b_key, n_bits, m_bits + 1);
  printf(
      "Matrices written to criteria_a_text.csv, criteria_b_text.csv, "
      "criteria_a_key.csv, criteria_b_key.csv\n");

  free_matrices(a_text, b_text, a_key, b_key, n_bits);
  free(padded);
  free(Y_all);

  return SUCCESS;
}

static int run_criteria_gost(void) {
  const int n_bits = 64;
  const int m_bits = 64;

  uint8_t* data = NULL;
  uint8_t* padded = NULL;
  uint8_t* Y_all = NULL;
  uint32_t** a_text = NULL;
  uint32_t** b_text = NULL;
  uint32_t** a_key = NULL;
  uint32_t** b_key = NULL;

  data = malloc(BUFFER_SIZE);
  if (!data) {
    perror("malloc");
    return FAILURE;
  }

  int fd = open(MESSAGE_FILE, O_RDONLY);
  if (fd == -1) {
    perror("open message");
    free(data);
    return FAILURE;
  }
  ssize_t n = read(fd, data, BUFFER_SIZE);
  close(fd);
  if (n < 0) n = 0;
  size_t data_len = (size_t)n;

  size_t N = (data_len + GOST_BLOCK_BYTES - 1) / GOST_BLOCK_BYTES;
  if (N == 0) N = 1;
  size_t padded_len = N * GOST_BLOCK_BYTES;

  padded = calloc(padded_len, 1);
  if (!padded) {
    perror("calloc");
    free(data);
    return FAILURE;
  }
  memcpy(padded, data, data_len);
  free(data);
  data = NULL;

  uint8_t key[GOST_KEY_BYTES];
  fd = open(KEY_FILE, O_RDONLY);
  if (fd == -1) {
    perror("open key");
    free(padded);
    return FAILURE;
  }
  memset(key, 0, GOST_KEY_BYTES);
  read(fd, key, GOST_KEY_BYTES);
  close(fd);

  gost_init();

  Y_all = malloc(N * GOST_BLOCK_BYTES);
  if (!Y_all) {
    perror("malloc");
    free(padded);
    return FAILURE;
  }

  for (size_t b = 0; b < N; b++)
    gost_encrypt_block(padded + b * GOST_BLOCK_BYTES,
                       Y_all + b * GOST_BLOCK_BYTES, key);

  a_text = calloc((size_t)n_bits, sizeof(uint32_t*));
  b_text = calloc((size_t)n_bits, sizeof(uint32_t*));
  a_key = calloc((size_t)n_bits, sizeof(uint32_t*));
  b_key = calloc((size_t)n_bits, sizeof(uint32_t*));

  if (!a_text || !b_text || !a_key || !b_key) {
    perror("calloc");
    free(padded);
    free(Y_all);
    free_matrices(a_text, b_text, a_key, b_key, n_bits);
    return FAILURE;
  }

  for (int i = 0; i < n_bits; i++) {
    a_text[i] = calloc((size_t)m_bits, sizeof(uint32_t));
    b_text[i] = calloc((size_t)m_bits + 1, sizeof(uint32_t));
    a_key[i] = calloc((size_t)m_bits, sizeof(uint32_t));
    b_key[i] = calloc((size_t)m_bits + 1, sizeof(uint32_t));
  }

  for (int i = 0; i < n_bits; i++) {
    for (size_t b = 0; b < N; b++) {
      uint8_t X[GOST_BLOCK_BYTES];
      memcpy(X, padded + b * GOST_BLOCK_BYTES, GOST_BLOCK_BYTES);
      flip_bit(X, i);

      uint8_t Y[GOST_BLOCK_BYTES];
      gost_encrypt_block(X, Y, key);

      uint8_t* Yb = Y_all + b * GOST_BLOCK_BYTES;
      int w = count_diff_bits(Y, Yb, GOST_BLOCK_BYTES);
      b_text[i][w]++;

      for (int j = 0; j < m_bits; j++) {
        uint8_t x = (uint8_t)(Y[j / 8] ^ Yb[j / 8]);
        if (x & (1 << (7 - (j % 8)))) a_text[i][j]++;
      }
    }
  }

  for (int i = 0; i < n_bits; i++) {
    uint8_t key_mod[GOST_KEY_BYTES];
    memcpy(key_mod, key, GOST_KEY_BYTES);
    flip_bit(key_mod, i);

    for (size_t b = 0; b < N; b++) {
      uint8_t Y[GOST_BLOCK_BYTES];
      gost_encrypt_block(padded + b * GOST_BLOCK_BYTES, Y, key_mod);

      uint8_t* Yb = Y_all + b * GOST_BLOCK_BYTES;
      int w = count_diff_bits(Y, Yb, GOST_BLOCK_BYTES);
      b_key[i][w]++;

      for (int j = 0; j < m_bits; j++) {
        uint8_t x = (uint8_t)(Y[j / 8] ^ Yb[j / 8]);
        if (x & (1 << (7 - (j % 8)))) a_key[i][j]++;
      }
    }
  }

  double s1_text = 0, s1_key = 0;
  for (int i = 0; i < n_bits; i++) {
    double sj_t = 0, sj_k = 0;
    for (int j = 0; j <= m_bits; j++) {
      sj_t += (double)j * b_text[i][j];
      sj_k += (double)j * b_key[i][j];
    }
    s1_text += sj_t / (double)N;
    s1_key += sj_k / (double)N;
  }
  double d1_text = s1_text / n_bits;
  double d1_key = s1_key / n_bits;

  long zero_a_text = 0, zero_a_key = 0;
  for (int i = 0; i < n_bits; i++) {
    for (int j = 0; j < m_bits; j++) {
      if (a_text[i][j] == 0) zero_a_text++;
      if (a_key[i][j] == 0) zero_a_key++;
    }
  }
  double d2_text = 1.0 - (double)zero_a_text / ((double)n_bits * m_bits);
  double d2_key = 1.0 - (double)zero_a_key / ((double)n_bits * m_bits);

  double s3_text = 0, s3_key = 0;
  for (int i = 0; i < n_bits; i++) {
    double inner_t = 0, inner_k = 0;
    for (int j = 0; j <= m_bits; j++) {
      inner_t += 2.0 * j * b_text[i][j];
      inner_k += 2.0 * j * b_key[i][j];
    }
    s3_text += fabs(inner_t / (double)N - m_bits);
    s3_key += fabs(inner_k / (double)N - m_bits);
  }
  double d3_text = 1.0 - s3_text / ((double)n_bits * m_bits);
  double d3_key = 1.0 - s3_key / ((double)n_bits * m_bits);

  double s4_text = 0, s4_key = 0;
  for (int i = 0; i < n_bits; i++) {
    for (int j = 0; j < m_bits; j++) {
      s4_text += fabs(2.0 * a_text[i][j] / (double)N - 1.0);
      s4_key += fabs(2.0 * a_key[i][j] / (double)N - 1.0);
    }
  }
  double d4_text = 1.0 - s4_text / ((double)n_bits * m_bits);
  double d4_key = 1.0 - s4_key / ((double)n_bits * m_bits);

  printf("\n=== GOST 28147-89 Criteria (N = %zu blocks) ===\n", N);
  printf("Diffusion (text):  d1=%.4f  d2=%.4f  d3=%.4f  d4=%.4f\n", d1_text,
         d2_text, d3_text, d4_text);
  printf("Confusion (key):   d1=%.4f  d2=%.4f  d3=%.4f  d4=%.4f\n", d1_key,
         d2_key, d3_key, d4_key);

  save_matrix("criteria_gost_a_text.csv", a_text, n_bits, m_bits);
  save_matrix("criteria_gost_b_text.csv", b_text, n_bits, m_bits + 1);
  save_matrix("criteria_gost_a_key.csv", a_key, n_bits, m_bits);
  save_matrix("criteria_gost_b_key.csv", b_key, n_bits, m_bits + 1);

  free_matrices(a_text, b_text, a_key, b_key, n_bits);
  free(padded);
  free(Y_all);

  return SUCCESS;
}