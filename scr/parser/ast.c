#include "../include/ast.h"

ast_t *ast_init(size_t class_size){
    ast_t *ast = calloc(1, sizeof(ast_t));
    ast->classe = calloc(class_size, sizeof(struct tree));
    ast->class_size = class_size;
    return ast;
}

struct tree *tree_init(unsigned char type){
    struct tree *tree = calloc(1, sizeof(struct tree));
    tree->type = type;
    tree->data_i = 0;
    tree->left_derive = NULL;
    tree->right_value = NULL;

    return tree;
}

struct tree *tree_init_ID(unsigned char type, size_t data_i){
    struct tree *tree = calloc(1, sizeof(struct tree));
    tree->type = type;
    tree->data_i = data_i;
    tree->left_derive = NULL;
    tree->right_value = NULL;

    return tree;
}
void tree_print(struct tree *root, memory_t *mem, int spaces, FILE *f) {
    if (root == NULL) {
        return;
    }
    size_t x = 0;

    if(root->data_i != 0){
        for(int i = root->data_i; mem->tokens[i] != '\0'; ++x, ++i);
    }else{
        x+=token_size(root->type);
    }
    tree_print(root->right_value, mem, spaces+x, f);

    for (int i = 0; i < spaces; i++) {
        fprintf(f," ");
    }
    x = 0;
    if(root->data_i != 0){
        for(int i = root->data_i; mem->tokens[i] != '\0'; ++x, ++i) fprintf(f,"%c", mem->tokens[i]); 
    }else{
        token_print_string(root->type, f);
        x+=token_size(root->type);
    }
    fprintf(f,"\n");
    tree_print(root->left_derive, mem, spaces+x, f);
}
void ast_print_spaces(ast_t *ast, memory_t *mem, int spaces, FILE *f){
    for(int i = 0; i < ast->class_size; ++i){
        tree_print(ast->classe[i], mem, 0, f);
        fprintf(f,"\n\n\n");
    }
}

void ast_print(ast_t *ast, memory_t *mem, FILE *f){
    fprintf(f, "\nAbstract Syntax Tree from Syntax Analysis:\n\n");
    ast_print_spaces(ast, mem, 0, f);
}

