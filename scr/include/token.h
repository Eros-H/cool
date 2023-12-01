#include <stdio.h>
#include <ctype.h>
#include "arena.h"
#ifndef TOKEN
#define TOKEN
enum type_t : u1{
    INTEGER = 128,
    LITERAL, BOOLEAN,
    ID,
    OBJECT_ID,
    TYPE_ID,
    SELF,
    SELF_ID,
    CLASS,
    ELSE,
    FALSE,
    MAP,
    FI,
    IF,
    IN,
    INHERITS,
    ISVOID,
    LET,
    LOOP,
    POOL,
    THEN,
    WHILE,
    CASE,
    ESAC,
    NEW,
    OF,
    NOT,
    TRUE,
    PLUS,
    STAR,
    MINUS,
    SLASH,
    TILDE,
    ARROBA,
    ASSIGN,
    EQUAL,
    LESS,
    LESSEQ,
    LPAREN,
    RPAREN,
    LBRAK,
    COLON,
    COMMA,
    SEMICOLON,
    DOT,
    RBRAK,
    INVALID
};
typedef struct STRUCT_MEM{
    u1 *tokens;
    u64 arena_i;
    u64 class_size;
}memory_t;

memory_t *memory_init(arena_t *a, u64 size);
void token_print_string(int type, FILE *f);
u8 token_print_size(int type);
#endif
