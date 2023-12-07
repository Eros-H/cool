#include "ast.h"
#ifndef PARSER
#define PARSER
typedef struct{
    size_t mem_i;
    memory_t *stream; 
}parser_t;
parser_t *parser_init(memory_t *mem);
void parser_free(parser_t *parser);
void parser_parse(parser_t *parser, ast_t **ast);
struct tree *parser_ast_exps(parser_t *parser, struct tree *tree);
#endif
