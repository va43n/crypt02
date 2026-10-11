#ifndef LAB3_H
#define LAB3_H

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "aes/aes.h"
#include "constants.h"
#include "gost/gost.h"

int create_files_if_needed(void);
void init_random(void);

int get_user_input(char* buffer, size_t buffer_size);
int get_mode_from_input(char* buffer, int* mode);

void print_file_in_binary(char* buffer, size_t buffer_size);
void print_file_in_hex(char* buffer, size_t buffer_size);
void print_file_in_dec(char* buffer, size_t buffer_size);
void print_file_in_symbol(char* buffer, size_t buffer_size);

int convert_binary_to_symbol(char* binary, size_t buffer_size, char* symbols,
                             size_t* symbols_size);
int convert_dec_to_symbol(char* dec, size_t buffer_size, char* symbols,
                          size_t* symbols_size);
int convert_hex_to_symbol(char* hex, size_t buffer_size, char* symbols,
                          size_t* symbols_size);

void calculate_xor(char* message, size_t message_size, char* key,
                   size_t key_size, char* cipher);

int parse_task1_aes_input(char* buffer, size_t buffer_size, int* mode,
                          char* file_name, int* power, char* user_data);
int perform_task1_aes(char* buffer, size_t buffer_size);

void gost_encrypt_block(const uint8_t in[GOST_BLOCK_BYTES],
                        uint8_t out[GOST_BLOCK_BYTES],
                        const uint8_t key[GOST_KEY_BYTES]);
void gost_decrypt_block(const uint8_t in[GOST_BLOCK_BYTES],
                        uint8_t out[GOST_BLOCK_BYTES],
                        const uint8_t key[GOST_KEY_BYTES]);
void gost_encrypt_block_track(
    const uint8_t in[GOST_BLOCK_BYTES], uint8_t out[GOST_BLOCK_BYTES],
    const uint8_t key[GOST_KEY_BYTES],
    uint8_t states[GOST_ROUNDS + 1][GOST_BLOCK_BYTES]);
void gost_encrypt_buffer(const uint8_t* in, size_t in_len, uint8_t* out,
                         size_t* out_len, const uint8_t key[GOST_KEY_BYTES]);
void gost_decrypt_buffer(const uint8_t* in, size_t in_len, uint8_t* out,
                         size_t* out_len, const uint8_t key[GOST_KEY_BYTES]);

int perform_task2(char* buffer, size_t buffer_size);

int perform_task3(char* buffer, size_t buffer_size);

#endif