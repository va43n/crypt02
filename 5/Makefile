CC = gcc
CLANG_FORMAT = clang-format
CFLAGS = -Wall -Wextra -Werror -std=gnu11
OTHER_FLAGS = -lm

SRC = $(wildcard bigint/*.c) \
	  $(wildcard dh/*.c) \
	  $(wildcard primes/*.c) \
	  $(wildcard roots/*.c) \
	  $(wildcard *.c)
HDR = $(wildcard bigint/*.h) \
	  $(wildcard dh/*.h) \
	  $(wildcard primes/*.h) \
	  $(wildcard roots/*.h) \
	  $(wildcard *.h)

OBJ = $(SRC:.c=.o)
# TEXT_FILES = message.txt cipher.txt key.txt key1.txt key2.txt key3.txt init.txt

MANUAL_TEST_TARGET = crypt.exe

all: manual_testing

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

manual_testing: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(MANUAL_TEST_TARGET) $(OTHER_FLAGS)

format:
	$(CLANG_FORMAT) -i $(SRC) $(HDR)

check_styles:
	cppcheck --enable=all --suppress=missingIncludeSystem .

valgrind: manual_testing
	valgrind --tool=memcheck --leak-check=yes --quiet ./$(MANUAL_TEST_TARGET)

clean:
	@rm -rf $(OBJ) \
			*.o

fclean: clean
	@rm -rf $(MANUAL_TEST_TARGET)

.PHONY: all \
		manual_testing \
		clean \
		format \
		check_styles \
		valgrind
