#include <stdio.h>
#include <stdlib.h>
#include "gap_buffer.h"

static void print_gb(struct gap_buffer *gb) {
  size_t len = gap_buffer_get_len(gb);
  for (int i = 0; i < gap_buffer_get_len(gb); ++i) {
    char tmp = '_';
    gap_buffer_get_char(gb, i, &tmp);
    fprintf(stdout, "%c", tmp);
  }
  fprintf(stdout, "\n");
}

int main(int argc, char** argv){

  struct gap_buffer gb;
  if (!gap_buffer_init(&gb, 16)) {
    fprintf(stderr, "gap buffer initialization error\n");
    exit(1);
  }

  if (!gap_buffer_insert_word(&gb, 0, "test")) {
    fprintf(stderr, "gap buffer failed insertion\n");
    exit(1);
  }

  print_gb(&gb);

  gap_buffer_move_cursor(&gb, 2);

  gap_buffer_insert(&gb, '-');

  print_gb(&gb);

  gap_buffer_insert_word(&gb, 3, " interrupted -");

  print_gb(&gb);

  gap_buffer_delete(&gb);

  print_gb(&gb);

  gap_buffer_delete_seq(&gb, 14);

  print_gb(&gb);

  gap_buffer_move_cursor(&gb, 4);

  gap_buffer_insert(&gb, 's');

  print_gb(&gb);

  return EXIT_SUCCESS;
}
