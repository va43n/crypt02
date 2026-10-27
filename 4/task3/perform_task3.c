#include "../lab4.h"

static int flip_bit(uint8_t *buf, int bit_pos) {
  buf[bit_pos / 8] ^= (uint8_t)(1u << (7 - (bit_pos % 8)));
  return 0;
}

static int popcount_xor(const uint8_t *a, const uint8_t *b, size_t n) {
  int c = 0;
  for (size_t i = 0; i < n; i++) {
    uint8_t x = (uint8_t)(a[i] ^ b[i]);
    while (x) {
      c += x & 1;
      x >>= 1;
    }
  }
  return c;
}

static int read_bytes(const char *name, uint8_t *buf, size_t cap, size_t *len) {
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

#define AVAL_BLOCKS 3
#define AVAL_BYTES (AVAL_BLOCKS * AES_BLOCK_BYTES)
#define AVAL_BITS (AVAL_BYTES * 8)

static void load_msg_3blocks(uint8_t out[AVAL_BYTES]) {
  static uint8_t buf[BUFFER_SIZE];
  size_t len = 0;
  memset(out, 0, AVAL_BYTES);
  if (read_bytes(MESSAGE_FILE, buf, BUFFER_SIZE, &len) == FAILURE)
    return;
  memcpy(out, buf, len < AVAL_BYTES ? len : AVAL_BYTES);
}

static int run_ctr_avalanche(int target) {
  uint8_t msg[AVAL_BYTES] = {0};
  uint8_t key[16] = {0};
  uint8_t iv[16] = {0};

  load_msg_3blocks(msg);
  if (read_key16(KEY_FILE, key) == FAILURE)
    return FAILURE;
  if (read_key16(INIT_FILE, iv) == FAILURE)
    return FAILURE;

  uint8_t g_base[AVAL_BLOCKS][16];
  ctr_gamma3(key, iv, g_base[0], g_base[1], g_base[2]);

  uint8_t C_base[AVAL_BYTES];
  for (int b = 0; b < AVAL_BLOCKS; b++)
    for (int i = 0; i < 16; i++)
      C_base[b * 16 + i] = msg[b * 16 + i] ^ g_base[b][i];

  uint8_t P_base[AVAL_BYTES];
  for (int b = 0; b < AVAL_BLOCKS; b++)
    for (int i = 0; i < 16; i++)
      P_base[b * 16 + i] = C_base[b * 16 + i] ^ g_base[b][i];

  int total_bits = (target == 2 || target == 3) ? 128 : 384;

  char fname[64];
  const char *tag;
  if (target == 1) {
    tag = "ctr_avalanche_1.csv";
    snprintf(fname, sizeof(fname), "%s", tag);
  } else if (target == 2) {
    tag = "ctr_avalanche_2.csv";
    snprintf(fname, sizeof(fname), "%s", tag);
  } else if (target == 3) {
    tag = "ctr_avalanche_3.csv";
    snprintf(fname, sizeof(fname), "%s", tag);
  } else {
    tag = "ctr_avalanche_4.csv";
    snprintf(fname, sizeof(fname), "%s", tag);
  }

  int fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd == -1) {
    return FAILURE;
  }

  dprintf(fd, target == 4 ? "bit_pos,P1,P2,P3\n" : "bit_pos,C1,C2,C3\n");

  for (int bit = 0; bit < total_bits; bit++) {
    uint8_t msg_m[AVAL_BYTES];
    uint8_t key_m[16], iv_m[16];
    uint8_t C_m[AVAL_BYTES];
    uint8_t gamma_m[AVAL_BLOCKS][16];

    memcpy(msg_m, msg, AVAL_BYTES);
    memcpy(key_m, key, 16);
    memcpy(iv_m, iv, 16);
    memcpy(gamma_m, g_base, sizeof(gamma_m));

    if (target == 1) {
      flip_bit(msg_m, bit);
      for (int b = 0; b < AVAL_BLOCKS; b++)
        for (int i = 0; i < 16; i++)
          C_m[b * 16 + i] = msg_m[b * 16 + i] ^ g_base[b][i];
    } else if (target == 2) {
      flip_bit(key_m, bit);
      ctr_gamma3(key_m, iv, gamma_m[0], gamma_m[1], gamma_m[2]);
      for (int b = 0; b < AVAL_BLOCKS; b++)
        for (int i = 0; i < 16; i++)
          C_m[b * 16 + i] = msg[b * 16 + i] ^ gamma_m[b][i];
    } else if (target == 3) {
      flip_bit(iv_m, bit);
      ctr_gamma3(key, iv_m, gamma_m[0], gamma_m[1], gamma_m[2]);
      for (int b = 0; b < AVAL_BLOCKS; b++)
        for (int i = 0; i < 16; i++)
          C_m[b * 16 + i] = msg[b * 16 + i] ^ gamma_m[b][i];
    } else {
      memcpy(C_m, C_base, AVAL_BYTES);
      flip_bit(C_m, bit);
    }

    int d1, d2, d3;
    if (target == 4) {
      for (int b = 0; b < AVAL_BLOCKS; b++)
        for (int i = 0; i < 16; i++)
          C_m[b * 16 + i] = C_m[b * 16 + i] ^ g_base[b][i];

      d1 = popcount_xor(C_m, P_base, 16);
      d2 = popcount_xor(C_m + 16, P_base + 16, 16);
      d3 = popcount_xor(C_m + 32, P_base + 32, 16);
    } else {
      d1 = popcount_xor(C_m, C_base, 16);
      d2 = popcount_xor(C_m + 16, C_base + 16, 16);
      d3 = popcount_xor(C_m + 32, C_base + 32, 16);
    }

    dprintf(fd, "%d,%d,%d,%d\n", bit, d1, d2, d3);
  }

  close(fd);
  printf("  wrote %s\n", fname);
  return SUCCESS;
}

static int run_ede_avalanche(int bit_pos) {
  static uint8_t msg_raw[BUFFER_SIZE];
  size_t m_len = 0;

  if (read_bytes(MESSAGE_FILE, msg_raw, BUFFER_SIZE, &m_len) == FAILURE)
    return FAILURE;

  uint8_t P[16] = {0};
  memcpy(P, msg_raw, m_len < 16 ? m_len : 16);

  uint8_t k1[16], k2[16], k3[16];
  if (read_key16(KEY1_FILE, k1) == FAILURE)
    return FAILURE;
  if (read_key16(KEY2_FILE, k2) == FAILURE)
    return FAILURE;
  if (read_key16(KEY3_FILE, k3) == FAILURE)
    return FAILURE;

  uint8_t base_states[EDE_STATES][16];
  uint8_t base_out[16];
  ede_encrypt_block_track(P, base_out, k1, k2, k3, base_states);

  const char *tags[4] = {"p", "k1", "k2", "k3"};

  for (int t = 0; t < 4; t++) {
    uint8_t P_m[16], k1_m[16], k2_m[16], k3_m[16];
    memcpy(P_m, P, 16);
    memcpy(k1_m, k1, 16);
    memcpy(k2_m, k2, 16);
    memcpy(k3_m, k3, 16);

    if (t == 0)
      flip_bit(P_m, bit_pos);
    else if (t == 1)
      flip_bit(k1_m, bit_pos);
    else if (t == 2)
      flip_bit(k2_m, bit_pos);
    else
      flip_bit(k3_m, bit_pos);

    uint8_t mod_states[EDE_STATES][16];
    uint8_t mod_out[16];
    ede_encrypt_block_track(P_m, mod_out, k1_m, k2_m, k3_m, mod_states);

    char fname[96];
    snprintf(fname, sizeof(fname), "avalanche_ede_%s_bit%d.csv", tags[t],
             bit_pos);

    int fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
      return FAILURE;
    }

    dprintf(fd, "round,bits_changed\n");
    for (int r = 0; r < EDE_STATES; r++) {
      int d = popcount_xor(base_states[r], mod_states[r], 16);
      dprintf(fd, "%d,%d\n", r, d);
    }
    close(fd);
    printf("  wrote %s\n", fname);
  }
  return SUCCESS;
}

int perform_task3(char *buffer, size_t buffer_size) {
  (void)buffer_size;

  char mode[BUFFER_SIZE];
  int bit_pos = 0;
  memset(mode, 0, sizeof(mode));

  int n = sscanf(buffer, "%s %d", mode, &bit_pos);
  if (n < 1) {
    printf("ERROR: cannot parse input.\n");
    return FAILURE;
  }

  if (strcmp(mode, BACK_EXIT_OPTION) == 0)
    return SUCCESS;

  if (strcmp(mode, EXAMINE_CTR_P) == 0)
    return run_ctr_avalanche(1);
  if (strcmp(mode, EXAMINE_CTR_K) == 0)
    return run_ctr_avalanche(2);
  if (strcmp(mode, EXAMINE_CTR_IV) == 0)
    return run_ctr_avalanche(3);
  if (strcmp(mode, EXAMINE_CTR_C) == 0)
    return run_ctr_avalanche(4);

  if (strcmp(mode, EXAMINE_EDE_AVAL) == 0) {
    if (n < 2 || bit_pos < 0 || bit_pos > 127) {
      printf("ERROR: usage: avalanche <bit_pos 0..127>\n");
      return FAILURE;
    }
    return run_ede_avalanche(bit_pos);
  }

  printf("ERROR: mode is not found.\n");
  return FAILURE;
}