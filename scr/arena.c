#include "include/arena.h"
void arena_reset(arena_t *a) {
    a->offset = 0;
}
void arena_free(arena_t *a) {
    free(a->buffer);
}
void arena_init(arena_t *a, u64 length) {
    a->buffer = (u1*)malloc(length);
    a->length = length;
    a->offset = 0;
}
void *arena_alloc(arena_t *a, u64 size) {
    if (size <= a->length - a->offset) {
        void *ptr = &a->buffer[a->offset];
        a->offset += size;
        return ptr;
    }
    return NULL;
}
