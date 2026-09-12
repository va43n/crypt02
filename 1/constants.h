#define MESSAGE_FILE "message.txt"
#define CIPHER_FILE "cipher.txt"
#define KEY_FILE "key.txt"

#define TRUE 1
#define FALSE 0

#define SUCCESS 111
#define FAILURE -111

#define BUFFER_SIZE 1024

#define STANDARD_MENU                                                     \
  do {                                                                    \
    printf(                                                               \
        "\n\nChoose a mode:\n\t(1) Encrypt data from '%s' with key '%s' " \
        "and put new message "                                            \
        "in '%s';\n\t(2) qwerty;\n\t(3) qwerty;\n\t(0) Exit.\n> ",        \
        MESSAGE_FILE, KEY_FILE, CIPHER_FILE);                             \
  } while (0)

#define BINARY "2"
#define HEX "16"
#define SYMBOL "symbol"

#define XOR_ENCRYPT_OPTION "1"
#define XOR_ENCRYPT_OPTION_NUMBER 1

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
        "\n\nChoose a mode:\n\t(%d) Encrypt;\n\t(%s <%s/%s/%s>) Generate a "  \
        "specific part of the encryption process;\n\t(%s <%s/%s/%s/%s> "      \
        "<%s/%s/%s>) Show specific part of the encryption process in "        \
        "specific form;\n\t(%s <%s/%s/%s> "                                   \
        "<%s/%s/%s> <text>) Write some text in the specific file in the "     \
        "specific form;\n\t(0) Back.\n> ",                                    \
        XOR_ENCRYPT_OPTION_NUMBER, XOR_GEN_OPTION, MESSAGE_FILE, CIPHER_FILE, \
        KEY_FILE, XOR_SHOW_OPTION, MESSAGE_FILE, CIPHER_FILE, KEY_FILE,       \
        XOR_SHOW_ALL, BINARY, HEX, SYMBOL, XOR_WRITE_OPTION, MESSAGE_FILE,    \
        CIPHER_FILE, KEY_FILE, BINARY, HEX, SYMBOL);                          \
  } while (0)
