#ifndef GAP_BUFFER_H
#define GAP_BUFFER_H

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
#endif

struct gap_buffer {
  size_t cap;
  size_t len;
  size_t cursor_start;
  size_t cursor_end;
  char *buffer;
};

bool gap_buffer_init(struct gap_buffer *gb, size_t cap, size_t buf_size);
bool gap_buffer_move_cursor(struct gap_buffer *gb, size_t pos);
void gap_buffer_get_char(struct gap_buffer *gb, size_t pos, char* out);
size_t gap_buffer_get_len(struct gap_buffer *gb);
bool gap_buffer_insert(struct gap_buffer *gb, char c);
bool gap_buffer_insert_word(struct gap_buffer *gb, size_t pos, char* input);
bool gap_buffer_delete(struct gap_buffer *gb);
bool gap_buffer_delete_seq(struct gap_buffer *gb, size_t n);
void gap_buffer_free(struct gap_buffer *gb);

#endif
