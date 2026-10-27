#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MESSAGE_FILE "message.txt"
#define CIPHER_FILE "cipher.txt"
#define KEY_FILE "key.txt"
#define INIT_FILE "init.txt"
#define KEY1_FILE "key1.txt"
#define KEY2_FILE "key2.txt"
#define KEY3_FILE "key3.txt"

#define TRUE 1
#define FALSE 0

#define SUCCESS 111
#define FAILURE -111

#define BUFFER_SIZE 4096

#define BACK_EXIT_OPTION "0"
#define BACK_EXIT_OPTION_NUMBER 0

#define ENCRYPTION_MODE "1"
#define ENCRYPTION_MODE_NUMBER 1
#define DECRYPTION_MODE "2"
#define DECRYPTION_MODE_NUMBER 2
#define EXAMINE_MODE "3"
#define EXAMINE_MODE_NUMBER 3

#define STANDARD_MENU                                                          \
  do {                                                                         \
    printf("\n\nChoose a mode:\n"                                              \
           "\t(%s) Encrypt;\n"                                                 \
           "\t(%s) Decrypt;\n"                                                 \
           "\t(%s) Examine;\n"                                                 \
           "\t(%s) Exit.\n> ",                                                 \
           ENCRYPTION_MODE, DECRYPTION_MODE, EXAMINE_MODE, BACK_EXIT_OPTION);  \
  } while (0)

#define HEX "16"
#define SYMBOL "symbol"

#define MODE_CTR "1"
#define MODE_CTR_NUMBER 1
#define MODE_EDE "2"
#define MODE_EDE_NUMBER 2

#define MODE_SHOW "show"
#define MODE_SHOW_NUMBER 3
#define MODE_WRITE "write"
#define MODE_WRITE_NUMBER 4

#define SHOW_ALL "all"

#define ENCRYPTION_MENU                                                        \
  do {                                                                         \
    printf("\n\nAvailable files: %s/%s/%s/%s/%s/%s/%s\nChoose a mode:\n"       \
           "\t(%s) Encrypt with AES-128 CTR (uses %s + %s);\n"                 \
           "\t(%s) Encrypt with AES-128 EDE (uses %s, %s, %s);\n"              \
           "\t(%s <file> <%s/%s>) Show a file;\n"                              \
           "\t(%s <file> <%s/%s> <text>) Write text to file;\n"                \
           "\t(%s) Back.\n> ",                                                 \
           MESSAGE_FILE, CIPHER_FILE, KEY_FILE, INIT_FILE, KEY1_FILE,          \
           KEY2_FILE, KEY3_FILE, MODE_CTR, KEY_FILE, INIT_FILE, MODE_EDE,      \
           KEY1_FILE, KEY2_FILE, KEY3_FILE, MODE_SHOW, HEX, SYMBOL,            \
           MODE_WRITE, HEX, SYMBOL, BACK_EXIT_OPTION);                         \
  } while (0)

#define DECRYPTION_MENU                                                        \
  do {                                                                         \
    printf("\n\nChoose a mode:\n"                                              \
           "\t(%s) Decrypt with AES-128 CTR;\n"                                \
           "\t(%s) Decrypt with AES-128 EDE;\n"                                \
           "\t(%s) Back.\n> ",                                                 \
           MODE_CTR, MODE_EDE, BACK_EXIT_OPTION);                              \
  } while (0)

#define EXAMINE_CTR_P "1"
#define EXAMINE_CTR_P_NUMBER 1
#define EXAMINE_CTR_K "2"
#define EXAMINE_CTR_K_NUMBER 2
#define EXAMINE_CTR_IV "3"
#define EXAMINE_CTR_IV_NUMBER 3
#define EXAMINE_CTR_C "4"
#define EXAMINE_CTR_C_NUMBER 4
#define EXAMINE_EDE_AVAL "avalanche"
#define EXAMINE_EDE_AVAL_NUMBER 5

#define EXAMINE_MENU                                                           \
  do {                                                                         \
    printf("\n\nChoose a mode:\n"                                              \
           "\t(%s) CTR: Flip in message text;\n"                               \
           "\t(%s) CTR: Flip in key;\n"                                        \
           "\t(%s) CTR: Flip in IV;\n"                                         \
           "\t(%s) CTR: Flip in cipher text;\n"                                \
           "\t(%s <bit_pos[0..127]>) EDE: flip in P/k1/k2/k3;\n"               \
           "\t(%s) Back.\n> ",                                                 \
           EXAMINE_CTR_P, EXAMINE_CTR_K, EXAMINE_CTR_IV, EXAMINE_CTR_C,        \
           EXAMINE_EDE_AVAL, BACK_EXIT_OPTION);                                \
  } while (0)

#endif