#include "lexer.h"
#ifndef AST
#define AST
struct tree{
    unsigned char type;
    size_t data_i;
    struct tree *left_derive; 
    struct tree *right_value; 
};
typedef struct{
    struct tree **classe; 
    size_t class_size;
}ast_t;
ast_t *ast_init(size_t class_size);
struct tree *tree_init(unsigned char type);
struct tree *tree_init_ID(unsigned char type, size_t data_i);
void ast_print(ast_t *ast, memory_t *mem, FILE *f);
#endif
