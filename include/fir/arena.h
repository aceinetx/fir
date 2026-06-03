#ifndef FEATHER_ARENA_H
#define FEATHER_ARENA_H
#include "config.h"
#include <stddef.h>

FR_HEADER_BEGIN

typedef struct frArena {
  char *data, *p;
  size_t size;
} frArena;

frArena frArena_new(size_t size);
void frArena_free(frArena *arena);
void frArena_clear(frArena *arena);
void *frArena_alloc(frArena *arena, size_t size);

FR_HEADER_END

#endif
