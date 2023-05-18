#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void display_prompt() {
  static int first_time_prompt = 1;
  char *msg = "Simple Shell: ";

  if (first_time_prompt) {
    system("clear");
    first_time_prompt = 0;
  }
  write(STDIN_FILENO, msg, strlen(msg));
}
