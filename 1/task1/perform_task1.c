#include "../lab1.h"

int perform_task1(char* buffer, size_t buffer_size) {
  char file_name[BUFFER_SIZE], text[BUFFER_SIZE];
  int power, mode;

  if (parse_task1_input(buffer, BUFFER_SIZE, &mode, file_name, &power, text) ==
      FAILURE)
    return FAILURE;

  memset(buffer, 0, buffer_size);
  if (mode == XOR_SHOW_OPTION_NUMBER) {
    int fd = open(file_name, O_RDONLY);

    read(fd, buffer, buffer_size);
    size_t len = strlen(buffer);

    printf("\n======%s======\n", file_name);
    if (power == 2)
      print_file_in_binary(buffer, len);
    else if (power == 16)
      print_file_in_hex(buffer, len);
    else if (power == 0)
      print_file_in_symbol(buffer, len);
    printf("======%s======\n\n", file_name);

    close(fd);
  } else if (mode == XOR_WRITE_OPTION_NUMBER) {
    int fd = open(file_name, O_WRONLY | O_TRUNC);
    size_t len = strlen(text);

    int res = SUCCESS;
    char text_result[BUFFER_SIZE];
    size_t text_size = len;
    if (power == 2)
      res = convert_binary_to_symbol(text, len, text_result, &text_size);
    else if (power == 16)
      res = convert_hex_to_symbol(text, len, text_result, &text_size);
    else if (power == 0)
      memcpy(text_result, text, len);

    if (res == FAILURE) {
      close(fd);
      return FAILURE;
    }

    write(fd, text_result, text_size);
    close(fd);
  }

  return SUCCESS;
}