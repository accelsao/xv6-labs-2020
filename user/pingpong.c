#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int parent_fd[2];
  int child_fd[2];
  char buf[1];

  pipe(parent_fd);
  pipe(child_fd);
  if (fork() == 0) { // child
    close(parent_fd[1]);
    close(child_fd[0]);
    int pid = getpid();

    read(parent_fd[0], buf, 1);
    if (buf[0] == 'p') {
      printf("%d: received ping\n", pid);
    }
    write(child_fd[1], "c", 1);
    close(parent_fd[0]);
    close(child_fd[1]);
  } else {
    close(parent_fd[0]);
    close(child_fd[1]);
    int pid = getpid();
    write(parent_fd[1], "p", 1);
    read(child_fd[0], buf, 1);
    if (buf[0] == 'c') {
      printf("%d: received pong\n", pid);
    }
    close(parent_fd[1]);
    close(child_fd[0]);
  }
  exit(0);
}