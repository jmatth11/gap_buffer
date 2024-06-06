#include <stdio.h>
#include <stdlib.h>
#include "gap_buffer.h"

int main(int argc, char** argv){

  struct gap_buffer gb;
  if (!gap_buffer_init(&gb, 16, 16)) {
    fprintf(stderr, "gap buffer initialization error\n");
    exit(1);
  }

  if (!gap_buffer_insert_word(&gb, 0, "test")) {

  }

  return EXIT_SUCCESS;
}
