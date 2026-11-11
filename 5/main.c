#include "lab5.h"

static double elapsed_seconds(struct timespec *a, struct timespec *b) {
  return (b->tv_sec - a->tv_sec) + (b->tv_nsec - a->tv_nsec) / 1e9;
}

static int read_line(char *buf, int size) {
  if (!fgets(buf, size, stdin))
    return 0;
  buf[strcspn(buf, "\n")] = 0;
  return 1;
}

static void menu_generate_prime(void) {
  char line[64];
  int bits, t;
  printf("bits (2..128): ");
  if (!read_line(line, 64))
    return;
  bits = atoi(line);
  printf("t (MR rounds, min 5): ");
  if (!read_line(line, 64))
    return;
  t = atoi(line);

  struct timespec t0, t1;
  clock_gettime(CLOCK_MONOTONIC, &t0);
  u128 iter = 0;
  u128 p = generate_prime(bits, t, &iter);
  clock_gettime(CLOCK_MONOTONIC, &t1);

  char buf_p[64], buf_i[64];
  u128_to_str(p, buf_p);
  u128_to_str(iter, buf_i);
  printf("Prime:      %s\n", buf_p);
  printf("Iterations: %s\n", buf_i);
  printf("Time:       %.6f s\n", elapsed_seconds(&t0, &t1));
}

static void menu_primes_in_range(void) {
  char lo_s[64], hi_s[64];
  printf("lo: ");
  if (!read_line(lo_s, 64))
    return;
  printf("hi: ");
  if (!read_line(hi_s, 64))
    return;

  u128 lo, hi;
  if (u128_from_str(lo_s, &lo) < 0 || u128_from_str(hi_s, &hi) < 0) {
    printf("Invalid input\n");
    return;
  }

  struct timespec t0, t1;
  clock_gettime(CLOCK_MONOTONIC, &t0);
  u128 min_p = 0, max_p = 0;
  u128 count = print_primes_in_range(lo, hi, &min_p, &max_p);
  clock_gettime(CLOCK_MONOTONIC, &t1);

  char buf[64];
  printf("\n--- Statistics ---\n");
  u128_to_str(count, buf);
  printf("Count: %s\n", buf);
  if (count > 0) {
    u128_to_str(min_p, buf);
    printf("Min:   %s\n", buf);
    u128_to_str(max_p, buf);
    printf("Max:   %s\n", buf);
  }
  printf("Time:  %.6f s\n", elapsed_seconds(&t0, &t1));
}

static void menu_primitive_roots(void) {
  char n_s[64];
  printf("n: ");
  if (!read_line(n_s, 64))
    return;
  u128 n;
  if (u128_from_str(n_s, &n) < 0) {
    printf("Invalid\n");
    return;
  }

  u128 roots[MAX_PRIMITIVE_ROOTS];
  int count = 0;

  struct timespec t0, t1;
  clock_gettime(CLOCK_MONOTONIC, &t0);
  int res = find_primitive_roots(n, roots, &count);
  clock_gettime(CLOCK_MONOTONIC, &t1);

  if (res == -1) {
    printf("ERROR: cannot factor phi(n) in reasonable time.\n");
    return;
  }
  if (res == -2) {
    printf("Primitive roots do not exist for n = %s\n", n_s);
    return;
  }

  char buf[64];
  printf("Found %d primitive roots:\n", count);
  for (int i = 0; i < count; i++) {
    u128_to_str(roots[i], buf);
    printf("  %s\n", buf);
  }
  printf("Time: %.6f s\n", elapsed_seconds(&t0, &t1));
}

static void menu_dh(void) {
  char line[64];
  int bits, t;
  printf("bits (2..128): ");
  if (!read_line(line, 64))
    return;
  bits = atoi(line);
  printf("t (MR rounds, min 5): ");
  if (!read_line(line, 64))
    return;
  t = atoi(line);

  struct timespec t0, t1;
  clock_gettime(CLOCK_MONOTONIC, &t0);
  dh_run(bits, t);
  clock_gettime(CLOCK_MONOTONIC, &t1);
  printf("Time: %.6f s\n", elapsed_seconds(&t0, &t1));
}

int main(void) {
  srand(time(NULL));
  primes_init();

  while (1) {
    printf("\n\nChoose a mode:\n"
           "\t(1) Generate prime\n"
           "\t(2) Print all primes in range\n"
           "\t(3) Find primitive roots\n"
           "\t(4) Diffie-Hellman exchange\n"
           "\t(0) Exit\n> ");

    char buf[64];
    if (!read_line(buf, 64))
      break;
    int mode = atoi(buf);

    if (mode == 0)
      break;
    else if (mode == 1)
      menu_generate_prime();
    else if (mode == 2)
      menu_primes_in_range();
    else if (mode == 3)
      menu_primitive_roots();
    else if (mode == 4)
      menu_dh();
    else
      printf("Unknown mode\n");
  }
  return 0;
}