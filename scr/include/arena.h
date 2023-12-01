#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#ifndef ARENA
#define ARENA
typedef struct arena arena_t;
struct arena {
    u1      *buffer;
    u64     length;
    u64     offset;
};
void arena_reset(arena_t *a);
void arena_free(arena_t *a);
void arena_init(arena_t *a, u64 length);
void *arena_alloc(arena_t *a, u64 size);
#endif
