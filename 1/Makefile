CC = gcc
CLANG_FORMAT = clang-format
CFLAGS = -Wall -Wextra -Werror -std=gnu11

SRC = $(wildcard common/*.c) \
      $(wildcard task1/*.c) \
			$(wildcard task2/*.c) \
			$(wildcard task3/*.c) \
			$(wildcard *.c)
OBJ = $(SRC:.c=.o)
TEXT_FILES = message.txt cipher.txt key.txt

MANUAL_TEST_TARGET = crypt.exe

all: manual_testing

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

manual_testing: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(MANUAL_TEST_TARGET)

format:
	$(CLANG_FORMAT) -i *.h $(SRC)

check_styles:
	cppcheck --enable=all --suppress=missingIncludeSystem .

valgrind: manual_testing
	valgrind --tool=memcheck --leak-check=yes --quiet ./$(MANUAL_TEST_TARGET)

clean:
	@rm -rf $(OBJ) \
			$(MANUAL_TEST_TARGET) \
			*.o \
			$(TEXT_FILES)

.PHONY: all \
		manual_testing \
		clean \
		format \
		check_styles \
		valgrind
