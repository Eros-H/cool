#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "utils.h"
#ifndef TOKEN
#define TOKEN
enum type_t : u8{
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
    EXPRESSION,
    FEATURE,
    INVALID
};
typedef struct STRUCT_MEM{
    u8 *tokens;
    u64 arena_i;
    u64 class_size;
    u64 arena_size;
}memory_t;

memory_t *memory_init(u64 size);
void token_print_string(int type, FILE *f);
u1 token_size(int type);
#endif
