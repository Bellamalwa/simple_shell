#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void display_prompt() {
  static int first_time_prompt = 1;

  if (first_time_prompt) {
    /* NOTE: This keeps looping for what ever reason */
    const char *ansi_clear_screen = " [1;1H [2J\n";
    /* TODO: Change the ansi_clear_screen as its value not allowed in sdt=89 */
    write(STDOUT_FILENO, ansi_clear_screen, 12);
    first_time_prompt = 0;
  }
  putchar('#');
}
