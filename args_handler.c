#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void process_args(char commands[], char *params[]) {
  char line_characters[1024];
  int line_counter = 0, index = 0, sub_index = 0;
  char *word_array[100], *process_char;

  for (;;) {
    int _char = fgetc(stdin);
    line_characters[line_counter++] = (char)_char;
    if (_char == '\n')
      break;
  }

  if (line_counter == 1)
    return;
  process_char = strtok(line_characters, " \n");

  while (process_char != NULL) {
    word_array[index++] = strdup(process_char);
    process_char = strtok(NULL, " \n");
  }

  strcpy(commands, word_array[0]);

  for (sub_index = 0; sub_index < index; sub_index++) {
    params[sub_index] = word_array[sub_index];
  }
  params[index] = NULL;
}
