#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MESSAGE_FILE "message.txt"
#define CIPHER_FILE "cipher.txt"
#define KEY_FILE "key.txt"

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
    printf(                                                                    \
        "\n\nChoose a mode:\n\t(%s) Encrypt data from '%s' with key '%s' "     \
        "and put new message in '%s';\n\t(%s) Decrypt data from '%s' with "    \
        "key '%s' and put new message in '%s';\n\t(%s) Examine the results "   \
        "of AES (avalanche + criteria d1-d4);\n\t(%s) Exit.\n> ",              \
        ENCRYPTION_MODE, MESSAGE_FILE, KEY_FILE, CIPHER_FILE, DECRYPTION_MODE, \
        CIPHER_FILE, KEY_FILE, MESSAGE_FILE, EXAMINE_MODE, BACK_EXIT_OPTION);  \
  } while (0)

#define HEX "16"
#define SYMBOL "symbol"
#define BINARY "2"

#define AES_ENCRYPT_OPTION "1"
#define AES_ENCRYPT_OPTION_NUMBER 1

#define AES_SHOW_OPTION "show"
#define AES_SHOW_OPTION_NUMBER 3

#define AES_WRITE_OPTION "write"
#define AES_WRITE_OPTION_NUMBER 4

#define AES_SHOW_ALL "all"

#define ENCRYPTION_MENU                                                        \
  do {                                                                         \
    printf(                                                                    \
        "\n\nChoose a mode:\n\t(%d) Encrypt with AES-128;\n\t"                 \
        "(%s <%s/%s/%s> <%s/%s>) Show a file in a specific form;\n\t"          \
        "(%s <%s/%s/%s> <%s/%s> <text>) Write text into a file in a "          \
        "specific form;\n\t"                                                   \
        "(%s) Back.\n> ",                                                      \
        AES_ENCRYPT_OPTION_NUMBER, AES_SHOW_OPTION, MESSAGE_FILE, CIPHER_FILE, \
        KEY_FILE, HEX, SYMBOL, AES_WRITE_OPTION, MESSAGE_FILE, CIPHER_FILE,    \
        KEY_FILE, HEX, SYMBOL, BACK_EXIT_OPTION);                              \
  } while (0)

#define DECRYPTION_MENU                                        \
  do {                                                         \
    printf(                                                    \
        "\n\nChoose a mode:\n\t(%s) Decrypt with AES-128;\n\t" \
        "(%s) Back.\n> ",                                      \
        AES_ENCRYPT_OPTION, BACK_EXIT_OPTION);                 \
  } while (0)

#define AES_AVALANCHE_OPTION "1"
#define AES_AVALANCHE_OPTION_NUMBER 5

#define AES_CRITERIA_OPTION "aes"
#define AES_CRITERIA_OPTION_NUMBER 6

#define GOST_CRITERIA_OPTION "gost"
#define GOST_CRITERIA_OPTION_NUMBER 7

#define EXAMINE_MENU                                                           \
  do {                                                                         \
    printf(                                                                    \
        "\n\nChoose a mode:\n"                                                 \
        "\t(%s <bit_pos[0..127]>) Run avalanche research for the first block " \
        "(writes 2 CSVs);\n"                                                   \
        "\t(%s) Compute criteria d1-d4 for AES-128;\n"                         \
        "\t(%s) Compute criteria d1-d4 for GOST 28147-89;\n"                   \
        "\t(%s) Back.\n> ",                                                    \
        AES_AVALANCHE_OPTION, AES_CRITERIA_OPTION, GOST_CRITERIA_OPTION,       \
        BACK_EXIT_OPTION);                                                     \
  } while (0)

#endif