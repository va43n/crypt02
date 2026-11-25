#include "lab6.h"
#include <ctype.h>

static void hex_to_dec_str(const char *hex, char *dec_out) {
  mpz_t num;
  mpz_init_set_str(num, hex, 16);
  char *s = mpz_get_str(NULL, 10, num);
  strcpy(dec_out, s);
  free(s);
  mpz_clear(num);
}

static void dec_to_hex_str(const char *dec, char *hex_out) {
  mpz_t num;
  mpz_init_set_str(num, dec, 10);
  char *s = mpz_get_str(NULL, 16, num);
  strcpy(hex_out, s);
  free(s);
  mpz_clear(num);
}

static void handle_keygen(void) {
  mpz_t e, d, n;
  mpz_inits(e, d, n, NULL);

  char choice[16];
  printf("Generate randomly (1) or read p,q from pq.txt (2)? ");
  if (!fgets(choice, sizeof(choice), stdin))
    return;

  if (choice[0] == '2') {
    mpz_t p, q;
    mpz_inits(p, q, NULL);
    read_hex_pair("pq.txt", p, q);
    printf("Generating RSA keys from pq.txt...\n");
    rsa_generate_keys_from_pq(p, q, e, d, n);
    mpz_clears(p, q, NULL);
  } else {
    printf("Generating RSA keys randomly...\n");
    rsa_generate_keys(e, d, n);
  }

  write_hex_pair(PUBLIC_KEY_FILE, e, n);
  write_hex_pair(PRIVATE_KEY_FILE, d, n);

  printf("Keys generated and saved to %s and %s\n", PUBLIC_KEY_FILE,
         PRIVATE_KEY_FILE);
  mpz_clears(e, d, n, NULL);
}

static void handle_encrypt(void) {
  unsigned char msg[BUFFER_SIZE], cipher[BUFFER_SIZE * 2];
  size_t msg_len;
  msg_len = read_hex_as_bytes(MESSAGE_FILE, msg);
  if (msg_len == 0) {
    printf("Message is empty.\n");
    return;
  }

  unsigned char aes_key[16];
  FILE *f = fopen("/dev/urandom", "rb");
  if (f) {
    fread(aes_key, 1, 16, f);
    fclose(f);
  } else {
    for (int i = 0; i < 16; i++)
      aes_key[i] = rand() % 256;
  }

  write_bytes_as_hex(KEY_FILE, aes_key, 16);
  printf("AES key generated and saved to %s\n", KEY_FILE);

  size_t out_len = 0;
  aes_encrypt_buffer(msg, msg_len, cipher, &out_len, aes_key);
  write_bytes_as_hex(CIPHER_FILE, cipher, out_len);
  printf("Encrypted %zu bytes of text to %s.\n", out_len, CIPHER_FILE);

  mpz_t e, n, m, c;
  mpz_inits(e, n, m, c, NULL);
  read_hex_pair(PUBLIC_KEY_FILE, e, n);

  mpz_import(m, 16, 1, 1, 1, 0, aes_key);
  rsa_encrypt(c, m, e, n);
  write_hex_single(ENC_KEY_FILE, c);
  printf("Encrypted AES key saved to %s.\n", ENC_KEY_FILE);

  mpz_clears(e, n, m, c, NULL);
}

static void handle_decrypt(void) {
  mpz_t d, n, enc_k, dec_k;
  mpz_inits(d, n, enc_k, dec_k, NULL);

  read_hex_single(ENC_KEY_FILE, enc_k);
  read_hex_pair(PRIVATE_KEY_FILE, d, n);

  rsa_decrypt(dec_k, enc_k, d, n);

  unsigned char aes_key[16] = {0};
  size_t count = 0;
  mpz_export(aes_key + (16 - mpz_sizeinbase(dec_k, 256)), &count, 1, 1, 1, 0,
             dec_k);

  if (count < 16) {
    memmove(aes_key + (16 - count), aes_key, count);
    memset(aes_key, 0, 16 - count);
  }

  write_bytes_as_hex(KEY_FILE, aes_key, 16);
  printf("Decrypted AES key saved to %s\n", KEY_FILE);

  unsigned char cipher[BUFFER_SIZE], msg[BUFFER_SIZE * 2];
  size_t c_len, msg_len;
  c_len = read_hex_as_bytes(CIPHER_FILE, cipher);
  if (c_len == 0) {
    printf("Cipher is empty.\n");
    mpz_clears(d, n, enc_k, dec_k, NULL);
    return;
  }

  aes_decrypt_buffer(cipher, c_len, msg, &msg_len, aes_key);
  write_bytes_as_hex(MESSAGE_FILE, msg, msg_len);
  printf("Decrypted %zu bytes of text back to %s.\n", msg_len, MESSAGE_FILE);

  mpz_clears(d, n, enc_k, dec_k, NULL);
}

static void show_single_file(const char *file, const char *type) {
  char buf[BUFFER_SIZE];
  read_file_as_string(file, buf, sizeof(buf));

  printf("====== %s == %s ======\n", file, type);

  if (strlen(buf) == 0) {
    printf("(empty)\n");
  } else if (strcmp(type, HEX) == 0) {
    printf("%s\n", buf);
  } else if (strcmp(type, DEC) == 0) {
    char *token = strtok(buf, " ");
    int first = 1;
    while (token) {
      char dec_out[BUFFER_SIZE];
      hex_to_dec_str(token, dec_out);
      printf("%s%s", first ? "" : " ", dec_out);
      first = 0;
      token = strtok(NULL, " ");
    }
    printf("\n");
  } else if (strcmp(type, SYMBOL) == 0) {
    char clean[BUFFER_SIZE];
    int j = 0;
    for (int i = 0; buf[i]; i++) {
      if (!isspace((unsigned char)buf[i])) {
        clean[j++] = buf[i];
      }
    }
    clean[j] = '\0';

    int len = strlen(clean);
    char padded[BUFFER_SIZE];
    if (len % 2 != 0) {
      padded[0] = '0';
      strcpy(padded + 1, clean);
    } else {
      strcpy(padded, clean);
    }

    printf("\"");
    for (int i = 0; padded[i]; i += 2) {
      char hex_byte[3] = {padded[i], padded[i + 1], '\0'};
      unsigned int byte;
      sscanf(hex_byte, "%2x", &byte);
      if (byte >= 32 && byte < 127) {
        putchar(byte);
      } else {
        putchar(' ');
      }
    }
    printf("\"\n");
  } else if (strcmp(type, SHOW_ALL) == 0) {
    show_single_file(file, DEC);
    show_single_file(file, HEX);
    show_single_file(file, SYMBOL);
  } else {
    printf("Unknown format: %s\n", type);
  }
  printf("====== %s == %s ======\n", file, type);
}

static void handle_show(char *args) {
  char target[256], type[64];
  if (sscanf(args, "%255s %63s", target, type) != 2) {
    printf("Usage: show <file|%s> <%s|%s|%s|%s>\n", SHOW_ALL, DEC, HEX, SYMBOL,
           SHOW_ALL);
    return;
  }

  if (strcmp(target, SHOW_ALL) == 0) {
    const char *files[] = {MESSAGE_FILE,     CIPHER_FILE,  PUBLIC_KEY_FILE,
                           PRIVATE_KEY_FILE, ENC_KEY_FILE, KEY_FILE,
                           "pq.txt"};
    for (size_t i = 0; i < sizeof(files) / sizeof(files[0]); i++) {
      show_single_file(files[i], type);
    }
  } else {
    show_single_file(target, type);
  }
}

static void handle_write(char *args) {
  char file[256], type[64], text[BUFFER_SIZE];
  if (sscanf(args, "%255s %63s %4095[^\n]", file, type, text) != 3) {
    printf("Usage: write <file> <%s|%s|%s> <text>\n", DEC, HEX, SYMBOL);
    return;
  }

  char hex_buf[BUFFER_SIZE * 2];

  if (strcmp(type, HEX) == 0) {
    strcpy(hex_buf, text);
  } else if (strcmp(type, DEC) == 0) {
    char *token = strtok(text, " ");
    int first = 1;
    hex_buf[0] = '\0';
    while (token) {
      char hex_out[BUFFER_SIZE];
      dec_to_hex_str(token, hex_out);
      if (!first)
        strcat(hex_buf, " ");
      strcat(hex_buf, hex_out);
      first = 0;
      token = strtok(NULL, " ");
    }
  } else if (strcmp(type, SYMBOL) == 0) {
    hex_buf[0] = '\0';
    for (int i = 0; text[i]; i++) {
      char tmp[4];
      snprintf(tmp, sizeof(tmp), "%02x", (unsigned char)text[i]);
      strcat(hex_buf, tmp);
    }
  } else {
    printf("Unknown format: %s\n", type);
    return;
  }

  write_file_as_string(file, hex_buf);
  printf("Written to %s\n", file);
}

int main(void) {
  setvbuf(stdout, NULL, _IONBF, 0);
  srand((unsigned)time(NULL));

  create_files_if_needed();

  char buffer[BUFFER_SIZE];
  while (1) {
    printf("\n\nChoose a mode:\n"
           "\t(1) Generate RSA Keys\n"
           "\t(2) Encrypt %s\n"
           "\t(3) Decrypt %s\n"
           "\t(show <file> <mode>) Show file\n"
           "\t(write <file> <mode> <text>) Write in file\n"
           "\t(0) Exit\n> ",
           MESSAGE_FILE, CIPHER_FILE);

    if (!fgets(buffer, sizeof(buffer), stdin))
      break;
    buffer[strcspn(buffer, "\n")] = 0;

    char cmd[64];
    if (sscanf(buffer, "%63s", cmd) != 1)
      continue;

    if (strcmp(cmd, "0") == 0)
      break;
    else if (strcmp(cmd, "1") == 0)
      handle_keygen();
    else if (strcmp(cmd, "2") == 0)
      handle_encrypt();
    else if (strcmp(cmd, "3") == 0)
      handle_decrypt();
    else if (strcmp(cmd, "4") == 0 || strcmp(cmd, "show") == 0)
      handle_show(buffer + strlen(cmd));
    else if (strcmp(cmd, "5") == 0 || strcmp(cmd, "write") == 0)
      handle_write(buffer + strlen(cmd));
    else
      printf("Unknown mode.\n");
  }
  return 0;
}