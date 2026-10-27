#ifndef LAB4_H
#define LAB4_H

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "aes/aes.h"
#include "constants.h"
#include "modes/ctr.h"
#include "modes/ede.h"

int create_files_if_needed(void);
void init_random(void);

int get_user_input(char *buffer, size_t buffer_size);
int get_mode_from_input(char *buffer, int *mode);

void print_file_in_binary(char *buffer, size_t buffer_size);
void print_file_in_hex(char *buffer, size_t buffer_size);
void print_file_in_symbol(char *buffer, size_t buffer_size);

int convert_hex_to_symbol(char *hex, size_t buffer_size, char *symbols,
                          size_t *symbols_size);

int perform_task1(char *buffer, size_t buffer_size);

int perform_task2(char *buffer, size_t buffer_size);

int perform_task3(char *buffer, size_t buffer_size);

#endif