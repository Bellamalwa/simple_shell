#include "main.h"
#include <string.h>
#include <syscall.h>
#include <unistd.h>
#include <wait.h>
/*
 * The problem with  this is yes it displays the prompt, but won't execute the
 * commands
 * */
int main() {
  char cmd[100], actual_cmd[100], *params[20];
  char *globalPrograms[] = {(char *)"PATH=/bin", 0};
  while (1) {
    display_prompt();
    process_args(cmd, params);
    if (fork() != 0) {
      /* NOTE: This slowed down the loop but need to make it really halt */
      wait(NULL);
    } else {
      strcpy(cmd, "/bin/");
      strcat(cmd, actual_cmd);
      execve(cmd, params, globalPrograms);
    }
    if (strcmp(actual_cmd, "exit") == 0) {
      break;
    }
  }
  return (0);
}
