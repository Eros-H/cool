#include "token.h"
typedef struct STRUCT_LEXER{
    char *file;
    char c;
    u64 i;
    u64 file_size;
    u8 line;
    u8 column;
}lexer_t;

lexer_t *lexer_init(char *file, u64 file_size);
void lexer_free(lexer_t *lexer);
void lexer_scan(lexer_t *lexer, memory_t *mem);
void lexer_print(lexer_t *lexer, memory_t *mem, FILE* file);
