#include "gap_buffer.h"
#include <stdlib.h>
#include <string.h>

#define GAP_DEFAULT_BUFFER_SIZE 16

static bool resize(struct gap_buffer *gb) {
  const size_t new_size = ((double)gb->cap) * 1.7;
  char* tmp_buf = realloc(gb->buffer, sizeof(char) * new_size);
  if (tmp_buf == NULL) return false;
  gb->buffer = tmp_buf;
  gb->cap = new_size;
  return true;
}

static bool resize_buffer(struct gap_buffer *gb) {
  if (gb->len + GAP_DEFAULT_BUFFER_SIZE >= gb->cap) {
    if (!resize(gb)) {
      return false;
    }
  }
  for (size_t i = gb->len - 1, j = gb->cap - 1; i >= gb->cursor_end; --i, --j) {
    gb->buffer[j] = gb->buffer[i];
    if (i == gb->cursor_end) {
      gb->cursor_end = j;
      gb->len = gb->cap;
    }
  }
  return true;
}

bool gap_buffer_init(struct gap_buffer *gb, size_t buf_size) {
  if (gb == NULL) {
    return false;
  }
  if (buf_size <= 0) {
    buf_size = GAP_DEFAULT_BUFFER_SIZE;
  }
  gb->buffer = (char*)malloc(sizeof(char)*buf_size);
  if (gb->buffer == NULL) {
    return false;
  }
  gb->len = buf_size;
  gb->cap = buf_size;
  gb->cursor_start = 0;
  gb->cursor_end = buf_size;
  return true;
}

bool gap_buffer_move_cursor(struct gap_buffer *gb, size_t pos) {
  if (pos > gb->cap) return false;
  const size_t gap_len = gb->cursor_end - gb->cursor_start;
  if (pos > gb->cursor_start) {
    size_t pos_offset = pos + gap_len;
    if (pos_offset > gb->cap) {
      pos_offset = gb->cap;
    }
    const size_t start_idx = gb->cursor_end;
    for (size_t i = start_idx; i < pos_offset; ++i) {
      gb->buffer[gb->cursor_start] = gb->buffer[i];
      gb->cursor_start++;
    }
    gb->cursor_end = pos_offset;
  } else if (pos < gb->cursor_start) {
    const size_t start_idx = gb->cursor_start - 1;
    for (size_t i = start_idx; i >= pos; --i) {
      gb->buffer[gb->cursor_end - 1] = gb->buffer[i];
      gb->cursor_end--;
    }
    gb->cursor_start = pos;
  }
  return true;
}

void gap_buffer_get_char(struct gap_buffer *gb, size_t pos, char* out) {
  if (pos > gb->cap) return;
  size_t idx = pos;
  if (idx >= gb->cursor_start) {
    idx = pos + (gb->cursor_end - gb->cursor_start);
  }
  *out = gb->buffer[idx];
}

size_t gap_buffer_get_len(struct gap_buffer *gb) {
  return gb->len - (gb->cursor_end - gb->cursor_start);
}

bool gap_buffer_insert(struct gap_buffer *gb, char c) {
  if (gb->cursor_start == gb->cursor_end) {
    if (!resize_buffer(gb)) return false;
  }
  gb->buffer[gb->cursor_start] = c;
  gb->cursor_start++;
  return true;
}

bool gap_buffer_insert_word(struct gap_buffer *gb, size_t pos, char* input) {
  if (!gap_buffer_move_cursor(gb, pos)) return false;
  const size_t len = strlen(input);
  for (int i = 0; i < len; ++i) {
    if (!gap_buffer_insert(gb, input[i])) return false;
  }
  return true;
}

bool gap_buffer_delete(struct gap_buffer *gb) {
  if (gb->cursor_start == 0) return false;
  gb->cursor_start--;
  return true;
}

bool gap_buffer_delete_seq(struct gap_buffer *gb, size_t n) {
  if (n > gb->cursor_start) return false;
  gb->cursor_start -= n;
  return true;
}

void gap_buffer_free(struct gap_buffer *gb) {
  free(gb->buffer);
  gb->buffer = NULL;
  gb->len = 0;
  gb->cap = 0;
  gb->cursor_end = 0;
  gb->cursor_start = 0;
}

