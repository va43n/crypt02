#include "io.h"
#include "../constants.h"
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void write_bytes_as_hex(const char *filename, const unsigned char *buf,
                        size_t len) {
  char hex[BUFFER_SIZE * 2];
  for (size_t i = 0; i < len; i++)
    snprintf(hex + i * 2, 3, "%02x", buf[i]);
  hex[len * 2] = '\0';
  write_file_as_string(filename, hex);
}

size_t read_hex_as_bytes(const char *filename, unsigned char *buf) {
  char hex[BUFFER_SIZE * 2];
  read_file_as_string(filename, hex, sizeof(hex));
  size_t len = strlen(hex) / 2;
  for (size_t i = 0; i < len; i++) {
    unsigned int byte;
    sscanf(hex + i * 2, "%2x", &byte);
    buf[i] = (unsigned char)byte;
  }
  return len;
}

void create_files_if_needed(void) {
  const char *files[] = {MESSAGE_FILE,     CIPHER_FILE,  PUBLIC_KEY_FILE,
                         PRIVATE_KEY_FILE, ENC_KEY_FILE, KEY_FILE,
                         "pq.txt"};
  for (size_t i = 0; i < sizeof(files) / sizeof(files[0]); i++) {
    int fd = open(files[i], O_RDWR | O_CREAT, 0644);
    if (fd != -1)
      close(fd);
  }
}

void read_file_raw(const char *filename, unsigned char *buf, size_t *len) {
  int fd = open(filename, O_RDONLY);
  if (fd == -1) {
    perror(filename);
    *len = 0;
    return;
  }
  ssize_t n = read(fd, buf, BUFFER_SIZE);
  close(fd);
  *len = (n > 0) ? (size_t)n : 0;
}

void write_file_raw(const char *filename, const unsigned char *buf,
                    size_t len) {
  int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd == -1) {
    perror(filename);
    return;
  }
  if (len > 0)
    write(fd, buf, len);
  close(fd);
}

void read_file_as_string(const char *filename, char *buf, size_t max_len) {
  FILE *f = fopen(filename, "r");
  if (!f) {
    perror(filename);
    buf[0] = '\0';
    return;
  }
  if (!fgets(buf, max_len, f))
    buf[0] = '\0';
  fclose(f);
  size_t len = strlen(buf);
  while (len > 0 && (buf[len - 1] == '\n' || buf[len - 1] == '\r')) {
    buf[--len] = '\0';
  }
}

void write_file_as_string(const char *filename, const char *str) {
  FILE *f = fopen(filename, "w");
  if (!f) {
    perror(filename);
    return;
  }
  fprintf(f, "%s\n", str);
  fclose(f);
}

void read_hex_pair(const char *filename, mpz_t a, mpz_t b) {
  FILE *f = fopen(filename, "r");
  if (!f) {
    perror(filename);
    exit(1);
  }
  char buf1[BUFFER_SIZE], buf2[BUFFER_SIZE];
  if (fscanf(f, "%4095s %4095s", buf1, buf2) != 2) {
    printf("ERROR: cannot read hex pair from %s\n", filename);
    exit(1);
  }
  mpz_set_str(a, buf1, 16);
  mpz_set_str(b, buf2, 16);
  fclose(f);
}

void write_hex_pair(const char *filename, const mpz_t a, const mpz_t b) {
  FILE *f = fopen(filename, "w");
  if (!f) {
    perror(filename);
    exit(1);
  }
  char *s1 = mpz_get_str(NULL, 16, a);
  char *s2 = mpz_get_str(NULL, 16, b);
  fprintf(f, "%s %s\n", s1, s2);
  free(s1);
  free(s2);
  fclose(f);
}

void read_hex_single(const char *filename, mpz_t a) {
  FILE *f = fopen(filename, "r");
  if (!f) {
    perror(filename);
    exit(1);
  }
  char buf[BUFFER_SIZE];
  if (fscanf(f, "%4095s", buf) != 1) {
    printf("ERROR: cannot read hex from %s\n", filename);
    exit(1);
  }
  mpz_set_str(a, buf, 16);
  fclose(f);
}

void write_hex_single(const char *filename, const mpz_t a) {
  FILE *f = fopen(filename, "w");
  if (!f) {
    perror(filename);
    exit(1);
  }
  char *s = mpz_get_str(NULL, 16, a);
  fprintf(f, "%s\n", s);
  free(s);
  fclose(f);
}