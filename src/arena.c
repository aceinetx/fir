#include "fir/arena.h"
#include "fir/config.h"
#include <assert.h>
#include <stdlib.h>

frArena frArena_new(size_t size) {
  frArena arena;
  arena.p = arena.data = malloc(size);
  arena.size = size;
  return arena;
}

void frArena_free(frArena *arena) { free(arena->data); }

void frArena_clear(frArena *arena) { arena->p = arena->data; }

void *frArena_alloc(frArena *arena, size_t size) {
  size = FR_ALIGN(size, sizeof(void *));

  size_t remaining = (size_t)(arena->data + arena->size - arena->p);
  assert(remaining >= size);
  if (remaining < size)
    return NULL;

  void *p = arena->p;
  arena->p += size;
  return p;
}
