#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "constants.h"

// common/initialization.c
int create_files_if_needed(void);

// common/input_parse.c
int get_user_input(char* buffer, size_t buffer_size);
int get_mode_from_input(char* buffer, int* mode);

// common/power_operations.c
void print_file_in_binary(char* buffer, size_t buffer_size);
void print_file_in_hex(char* buffer, size_t buffer_size);
void print_file_in_symbol(char* buffer, size_t buffer_size);

int convert_binary_to_symbol(char* binary, size_t buffer_size, char* symbols,
                             size_t* symbols_size);
int convert_hex_to_symbol(char* hex, size_t buffer_size, char* symbols,
                          size_t* symbols_size);

// task1/parse_task1_input.c
int parse_task1_input(char* buffer, size_t buffer_size, int* mode,
                      char* file_name, int* power, char* user_data);

// task1/perform_task1.c
int perform_task1(char* buffer, size_t buffer_size);
