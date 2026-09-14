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

#define STANDARD_MENU                                                          \
  do {                                                                         \
    printf(                                                                    \
        "\n\nChoose a mode:\n\t(%s) Encrypt data from '%s' with key '%s' "     \
        "and put new message "                                                 \
        "in '%s';\n\t(%s) Decrypt data from '%s' with key '%s' and put new "   \
        "message in '%s';\n\t(%s) Exit.\n> ",                                  \
        ENCRYPTION_MODE, MESSAGE_FILE, KEY_FILE, CIPHER_FILE, DECRYPTION_MODE, \
        CIPHER_FILE, KEY_FILE, MESSAGE_FILE, BACK_EXIT_OPTION);                \
  } while (0)

#define BINARY "2"
#define HEX "16"
#define SYMBOL "symbol"
#define DEC "10"

#define XOR_ENCRYPT_OPTION "1"
#define XOR_ENCRYPT_OPTION_NUMBER 1

#define LFSR_ENCRYPT_OPTION "2"
#define LFSR_ENCRYPT_OPTION_NUMBER 2

#define ENCRYPTION_MENU                                                      \
  do {                                                                       \
    printf(                                                                  \
        "\n\nChoose a mode:\n\t(%s) Encrypt with XOR;\n\t(%s) Encrypt with " \
        "LFSR;\n\t(%s) Back.\n> ",                                           \
        XOR_ENCRYPT_OPTION, LFSR_ENCRYPT_OPTION, BACK_EXIT_OPTION);          \
  } while (0)

#define XOR_GEN_OPTION "gen"
#define XOR_GEN_OPTION_NUMBER 2
#define XOR_GEN_SIZE 32

#define XOR_SHOW_OPTION "show"
#define XOR_SHOW_OPTION_NUMBER 3
#define XOR_SHOW_ALL "all"

#define XOR_WRITE_OPTION "write"
#define XOR_WRITE_OPTION_NUMBER 4

#define XOR_ENCRYPTION_MENU                                                   \
  do {                                                                        \
    printf(                                                                   \
        "\n\nChoose a mode:\n\t(%d) Encrypt with XOR;\n\t(%s <%s/%s/%s>) "    \
        "Generate a "                                                         \
        "specific part of the encryption process;\n\t(%s <%s/%s/%s/%s> "      \
        "<%s/%s/%s>) Show specific part of the encryption process in "        \
        "specific form;\n\t(%s <%s/%s/%s> "                                   \
        "<%s/%s/%s> <text>) Write some text in the specific file in the "     \
        "specific form;\n\t(%s) Back.\n> ",                                   \
        XOR_ENCRYPT_OPTION_NUMBER, XOR_GEN_OPTION, MESSAGE_FILE, CIPHER_FILE, \
        KEY_FILE, XOR_SHOW_OPTION, MESSAGE_FILE, CIPHER_FILE, KEY_FILE,       \
        XOR_SHOW_ALL, BINARY, HEX, SYMBOL, XOR_WRITE_OPTION, MESSAGE_FILE,    \
        CIPHER_FILE, KEY_FILE, BINARY, HEX, SYMBOL, BACK_EXIT_OPTION);        \
  } while (0)

#define LFSR_SHOW_OPTION "show"
#define LFSR_SHOW_OPTION_NUMBER 3

#define LFSR_WRITE_OPTION "write"
#define LFSR_WRITE_OPTION_NUMBER 4

#define LFSR_LENGTH 8
#define LFSR_1 "x^8 + x^7 + x^6 + x^3 + x^2 + 1"
#define LFSR_1_SHIFTS 0b11001101
#define LFSR_1_NUMBER 1

#define LFSR_2 "x^8 + x^5 + x^3 + x^2 + 1"
#define LFSR_2_SHIFTS 0b10101101
#define LFSR_2_NUMBER 2

#define LFSR_ALL_LFSRS \
  { LFSR_1_SHIFTS, LFSR_2_SHIFTS }

#define LFSR_PICK_OPTION "pick"
#define LFSR_PICK_OPTION_NUMBER 5

#define LFSR_ENCRYPTION_MENU                                                   \
  do {                                                                         \
    printf(                                                                    \
        "\n\nChoose a mode:\n\t(%d) Encrypt with LFSR;\n\t(%s <%s> "           \
        "<%s/%s/%s>) show the initial value in a specific form\n\t(%s "        \
        "<%s/%s> <%s/%s/%s>) show a specific part of the encryption process "  \
        "in a specific form\n\t(%s <%s> <%s/%s/%s>) write the initial value "  \
        "in a specific form\n\t(%s <%s/%s> <%s/%s/%s>) write a specific part " \
        "of the encryption process in a specific form\n\t(%s <%d/%d>) Pick "   \
        "one of the LFSR polinomes: %s or %s (current - %d)\n\t(%s) "          \
        "Back.\n> ",                                                           \
        LFSR_ENCRYPT_OPTION_NUMBER, LFSR_SHOW_OPTION, KEY_FILE, BINARY, HEX,   \
        DEC, LFSR_SHOW_OPTION, MESSAGE_FILE, CIPHER_FILE, BINARY, HEX, SYMBOL, \
        LFSR_WRITE_OPTION, KEY_FILE, BINARY, HEX, DEC, LFSR_WRITE_OPTION,      \
        MESSAGE_FILE, CIPHER_FILE, BINARY, HEX, SYMBOL, LFSR_PICK_OPTION,      \
        LFSR_1_NUMBER, LFSR_2_NUMBER, LFSR_1, LFSR_2, current_lfsr_number,     \
        BACK_EXIT_OPTION);                                                     \
  } while (0)
