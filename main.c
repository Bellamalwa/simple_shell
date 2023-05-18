#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <syscall.h>
#include <unistd.h>
#define MAX_LINE_LENGTH BUFSIZ
/*
 * The problem with  this is yes it displays the prompt, but won't execute the
 * commands
 * */
int main() {
  char *environ[] = {(char *)"PATH=/bin", 0};
  char cmd[100], command[100], *params[20];
  while (1) {
    display_prompt(1);
    process_args(command, params);
    /*NOTE: This helps control the screen to display prompt */
    /*
    getline(argv, &buf_size, stdin);
    */
    if (fork() != 0) {
      wait(NULL);
    } else {
      /*TODO: Split this into a child Process */
      strcpy(cmd, "/bin/");
      strcat(cmd, command);
      execve(cmd, params, environ);
    }
    if (strcmp(command, "exit") == 0) {
      break;
    }
  }
  return (0);
}
