#include "include/compile.h"
#include <stdio.h>
#include <string.h>

char *read_file(char *file, u64 *file_size) { FILE *arq = fopen(file, "rb");
    if (arq == NULL) {
        printf("Error in reading file\n");
        exit(1);
    }

    fseek(arq, 0, SEEK_END);
    *file_size = ftell(arq);
    fseek(arq, 0, SEEK_SET);

    char *program = (char *)malloc(*file_size + 1);
    if (program == NULL) {
        fclose(arq);
        printf("Memory allocation error\n");
        exit(1);
    }

    u64 bytes_read = fread(program, 1, *file_size, arq);
    program[bytes_read] = '\0';

    fclose(arq);
    return program;
}

char *file_out(char *file) {
    char path[27] = "../testcases/debug/";
    char *new_file;
    char *token = strrchr(file, '/');  
    char *first_part = strtok(++token, "."); 

    char lexical_sufix[13] = "_debug.txt";
    u64 new_file_size = strlen(path) + strlen(first_part) + strlen(lexical_sufix) + 1;

    new_file = (char *)malloc(new_file_size);
    snprintf(new_file, new_file_size, "%s%s%s", path, first_part, lexical_sufix);


    return new_file;
}

void compile(char *file){

    u64 file_size = 0;

    char *file_buffer = read_file(file, &file_size);
    char *file_debugger = file_out(file);
    FILE *f = fopen(file_debugger, "w");

    
    memory_t *mem = memory_init(file_size*3);
    lexer_t *lexer = lexer_init(file_buffer, file_size); 
    lexer_scan(lexer, mem); 
    lexer_print(lexer, mem, f);

    ast_t *ast = ast_init(mem->class_size); 
    parser_t *parser = parser_init(mem);
    parser_parse(parser, &ast);
    ast_print(ast, mem, stdout);
    
    if(f) fclose(f);
    free(file_debugger);
    
    lexer_free(lexer);
}


