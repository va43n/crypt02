#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "constants.h"

extern int current_lfsr_number;

// common/initialization.c
int create_files_if_needed(void);
void init_random(void);

// common/input_parse.c
int get_user_input(char* buffer, size_t buffer_size);
int get_mode_from_input(char* buffer, int* mode);

// common/power_operations.c
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

// common/xor.c
void calculate_xor(char* message, size_t message_size, char* key,
                   size_t key_size, char* cipher);

// task1/parse_task1_xor_input.c
int parse_task1_xor_input(char* buffer, size_t buffer_size, int* mode,
                          char* file_name, int* power, char* user_data);

// task1/parse_task1_lfsr_input.c
int parse_task1_lfsr_input(char* buffer, size_t buffer_size, int* mode,
                           char* file_name, int* power, char* user_data);

// task1/perform_task1_xor.c
int perform_task1_xor(char* buffer, size_t buffer_size);

// task1/perform_task1_lfsr.c
void generate_key_with_lfsr(char* key_buffer, size_t key_len,
                            unsigned char initial_value);
int perform_task1_lfsr(char* buffer, size_t buffer_size);

// task2/perform_task2.c
int perform_task2(char* buffer, size_t buffer_size);
