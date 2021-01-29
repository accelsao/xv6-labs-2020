#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  char args[MAXARG][32];
  char *commands[MAXARG];
  if (argc < 2) {
    fprintf(2, "Usage: xargs command ...\n");
    exit(1);
  }

  while (1) {
    memset(args, 0, sizeof args);
    int j = 0;
    for (int i = 1; i < argc && i + 1 < MAXARG; i++) {
      strcpy(args[j++], argv[i]);
    }
    char ch;
    int n;
    int p = 0;
    int flag = 0;
    while ((n = read(0, &ch, 1)) > 0 && ch != '\n' && j + 1 < MAXARG) {
      if (ch == ' ') {
        if (flag) {
          flag = 0;
          p = 0;
          j++;
        }
      } else {
        args[j][p++] = ch;
        flag = 1;
      }
    }
    if (n <= 0) {
      wait(0);
      exit(0);
    }

    for (int i = 0; i + 1 < MAXARG; i++) {
      commands[i] = args[i];
    }
    commands[MAXARG - 1] = 0;
    if (fork() == 0) {
      exec(commands[0], commands);
      exit(0);
    }
  }

  exit(0);
}