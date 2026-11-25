#ifndef IO_H
#define IO_H

#include <gmp.h>
#include <stddef.h>

void create_files_if_needed(void);
void read_file_raw(const char *filename, unsigned char *buf, size_t *len);
void write_file_raw(const char *filename, const unsigned char *buf, size_t len);

void read_file_as_string(const char *filename, char *buf, size_t max_len);
void write_file_as_string(const char *filename, const char *str);

void read_hex_pair(const char *filename, mpz_t a, mpz_t b);
void write_hex_pair(const char *filename, const mpz_t a, const mpz_t b);
void read_hex_single(const char *filename, mpz_t a);
void write_hex_single(const char *filename, const mpz_t a);

void write_bytes_as_hex(const char *filename, const unsigned char *buf,
                        size_t len);
size_t read_hex_as_bytes(const char *filename, unsigned char *buf);

#endif