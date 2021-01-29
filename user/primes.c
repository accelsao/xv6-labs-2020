#include "kernel/types.h"
#include "user/user.h"

void seive() {
  int p[2], n;
loop:

  if (!read(0, &n, sizeof(int)) || !n) {
    close(0);
    exit(0);
  }

  printf("prime %d\n", n);
  pipe(p);

  if (fork() == 0) {

    close(0);
    dup(p[0]);
    close(p[0]);
    close(p[1]);
    goto loop;
  }

  close(p[0]);
  int num;
  while (read(0, &num, sizeof(int)) > 0) {
    if (num % n) {
      write(p[1], &num, sizeof(int));
    }
  }
  close(0);
  close(p[1]);
  wait(0);
  exit(0);
}

int main(int argc, char *argv[]) {
  int p[2];
  pipe(p);
  if (fork() == 0) {
    close(0);
    dup(p[0]);
    close(p[0]);
    close(p[1]);
    seive();
  } else {
    close(p[0]);
    for (int i = 2; i <= 35; i++) {
      write(p[1], &i, sizeof(int));
    }
    close(p[1]);
    wait(0);
  }
  exit(0);
}