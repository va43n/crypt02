#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MESSAGE_FILE "message.txt"
#define CIPHER_FILE "cipher.txt"
#define KEY_FILE "key.txt"

#define TRUE 1
#define FALSE 0

#define SUCCESS 111
#define FAILURE -111

#define BUFFER_SIZE 1024

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
        "of the Feistel network;\n\t(%s) Exit.\n> ",                           \
        ENCRYPTION_MODE, MESSAGE_FILE, KEY_FILE, CIPHER_FILE, DECRYPTION_MODE, \
        CIPHER_FILE, KEY_FILE, MESSAGE_FILE, EXAMINE_MODE, BACK_EXIT_OPTION);  \
  } while (0)

#define BINARY "2"
#define HEX "16"
#define SYMBOL "symbol"
#define DEC "10"

#define FEISTEL_ENCRYPT_OPTION "1"
#define FEISTEL_ENCRYPT_OPTION_NUMBER 1

#define FEISTEL_SHOW_OPTION "show"
#define FEISTEL_SHOW_OPTION_NUMBER 3

#define FEISTEL_WRITE_OPTION "write"
#define FEISTEL_WRITE_OPTION_NUMBER 4

#define FEISTEL_PICK_KEY_METHOD_OPTION "pick_key"
#define FEISTEL_PICK_KEY_METHOD_OPTION_NUMBER 5

#define FEISTEL_PICK_FUNC_OPTION "pick_func"
#define FEISTEL_PICK_FUNC_OPTION_NUMBER 6

#define FEISTEL_SHOW_ALL "all"

#define FEISTEL_ENCRYPTION_MENU                                                \
  do {                                                                         \
    printf(                                                                    \
        "\n\nChoose a mode (current key method: %d, current func: %d):\n"      \
        "\t(%d) Encrypt with Feistel;\n"                                       \
        "\t(%s <%s/%s/%s> <%s/%s/%s>) Show a file in a specific form;\n"       \
        "\t(%s <%s/%s/%s> <%s/%s/%s> <text>) Write text into a file in a "     \
        "specific form;\n"                                                     \
        "\t(%s <%d/%d>) Pick a subkey generation method (1 = cyclic 32 bits "  \
        "from key, 2 = 8 bits -> LFSR -> 32 bits);\n"                          \
        "\t(%s <%d/%d>) Pick a round function (1 = F(V)=V, 2 = F(R,V) = "      \
        "S(R) XOR V);\n"                                                       \
        "\t(%s) Back.\n> ",                                                    \
        current_key_method, current_func_type, FEISTEL_ENCRYPT_OPTION_NUMBER,  \
        FEISTEL_SHOW_OPTION, MESSAGE_FILE, CIPHER_FILE, KEY_FILE, BINARY, HEX, \
        SYMBOL, FEISTEL_WRITE_OPTION, MESSAGE_FILE, CIPHER_FILE, KEY_FILE,     \
        BINARY, HEX, SYMBOL, FEISTEL_PICK_KEY_METHOD_OPTION,                   \
        FEISTEL_KEY_METHOD_A, FEISTEL_KEY_METHOD_B, FEISTEL_PICK_FUNC_OPTION,  \
        FEISTEL_FUNC_A, FEISTEL_FUNC_B, BACK_EXIT_OPTION);                     \
  } while (0)

#define DECRYPTION_MENU                                                     \
  do {                                                                      \
    printf(                                                                 \
        "\n\nChoose a mode:\n\t(%s) Decrypt with Feistel network;\n\t(%s) " \
        "Back.\n> ",                                                        \
        FEISTEL_ENCRYPT_OPTION, BACK_EXIT_OPTION);                          \
  } while (0)

#define EXAMINE_MENU                                                        \
  do {                                                                      \
    printf(                                                                 \
        "\n\nChoose a mode:\n\t(%s <bit_pos[0..63]>) Run avalanche-effect " \
        "research for the given bit position (8 CSVs);\n"                   \
        "\t(%s) Back.\n> ",                                                 \
        EXAMINE_AVALANCHE_OPTION, BACK_EXIT_OPTION);                        \
  } while (0)

#define EXAMINE_AVALANCHE_OPTION "1"
#define EXAMINE_AVALANCHE_OPTION_NUMBER 1

#define FEISTEL_KEY_METHOD_A 1
#define FEISTEL_KEY_METHOD_B 2

#define FEISTEL_FUNC_A 1
#define FEISTEL_FUNC_B 2

#endif