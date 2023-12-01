#include "../include/lexer.h"
#include <ctype.h>
#include <stdio.h>

char ispun(char c){return c == '@' || c == '.' || c == '+' || c == '-' || c == '/' || c == '*' || c == '~' || c == '=' || c == '<' || c == ':' || c == ';' || c == ',' || c == '(' || c == ')' || c == '{' || c == '}';}
char ispeeker(char c){ return isspace(c) || ispun(c);}

lexer_t *lexer_init(char *file, u64 file_size){
    lexer_t *lexer = calloc(1, sizeof(struct STRUCT_LEXER)); 
    lexer->file = file; 
    lexer->file_size = file_size;
    lexer->c = lexer->file[0];
    lexer->i = 0;

    return lexer;
}

void lexer_free(lexer_t *lexer){
    free(lexer->file);
    free(lexer);
}

char lexer_peek(lexer_t *lexer, u64 i){
    return lexer->file[(lexer->i+i < lexer->file_size) ? lexer->i+i : lexer->file_size-1];
}

void lexer_print(lexer_t *lexer, memory_t *mem, FILE* file){
    fprintf(file, "Token Stream from Lexical Analysis:\n");
    for(int i = 0; i < mem->arena_i; ++i){
        if(mem->tokens[i] == LITERAL || mem->tokens[i] == TYPE_ID || mem->tokens[i] == OBJECT_ID || mem->tokens[i] == INTEGER){
            fprintf(file , "Token(%i): ", mem->tokens[i]);
            token_print_string(mem->tokens[i], file);
            fprintf(file , "<");
            while(mem->tokens[++i] != '\0'){
                fprintf(file, "%c", mem->tokens[i]);
            }
            fprintf(file , ">\n");
        }else{
            fprintf(file , "Token(%i): ", mem->tokens[i]);
            token_print_string(mem->tokens[i], file);
            fprintf(file, "\n");
        }
    }
}

void lexer_eat_n(lexer_t *lexer, u64 n){
    if(lexer->i < lexer->file_size && lexer->c != '\0'){
        lexer->i += n;
        lexer->c = lexer->file[lexer->i];
    }
}

void lexer_eat_arena(lexer_t *lexer, memory_t *mem, u1 type, u64 n){ 
    if(type == CLASS) mem->class_size++;
    mem->tokens[mem->arena_i++] = type;
    lexer_eat_n(lexer, n);
}

void lexer_eat_keyword(lexer_t *lexer, memory_t *mem){        
    switch(lexer->c){
        case 'c':{
                     if(lexer_peek(lexer,1) == 'a')if(lexer_peek(lexer,2) == 's')if(lexer_peek(lexer,3) == 'e')
                         if(ispeeker(lexer_peek(lexer,4))){
                             lexer_eat_arena(lexer, mem, CASE, 4);
                         }
                     if(lexer_peek(lexer,1) == 'l')if(lexer_peek(lexer,2) == 'a')if(lexer_peek(lexer,3) == 's')if(lexer_peek(lexer,4) == 's')if(ispeeker(lexer_peek(lexer,5))){
                         lexer_eat_arena(lexer, mem, CLASS, 5);
                     }
                 }break;
        case 'i':{
                     if(lexer_peek(lexer,1) == 'n')if(lexer_peek(lexer,2) == 'h')if(lexer_peek(lexer,3) == 'e')
                         if(lexer_peek(lexer,4) == 'r')if(lexer_peek(lexer,5) == 'i')if(lexer_peek(lexer,6) == 't')
                             if(lexer_peek(lexer,7) == 's')if(ispeeker(lexer_peek(lexer,8))){
                                 lexer_eat_arena(lexer, mem, INHERITS, 8);
                             }
                     if(lexer_peek(lexer,1) == 's')if(lexer_peek(lexer,2) == 'v')if(lexer_peek(lexer,3) == 'o')
                         if(lexer_peek(lexer,4) == 'i')if(lexer_peek(lexer,5) == 'd')if(ispeeker(lexer_peek(lexer,6))){
                             lexer_eat_arena(lexer, mem, ISVOID, 6);
                         }
                     if(lexer_peek(lexer,1) == 'n')if(ispeeker(lexer_peek(lexer,2))){
                         lexer_eat_arena(lexer, mem, IN, 2);
                     }
                     if(lexer_peek(lexer,1) == 'f')if(ispeeker(lexer_peek(lexer,2))){
                         lexer_eat_arena(lexer, mem, IF, 2);
                     }
                 }break;
        case 'l':{
                     if(lexer_peek(lexer,1) == 'e')if(lexer_peek(lexer,2) == 't')if(ispeeker(lexer_peek(lexer,3))){
                         lexer_eat_arena(lexer, mem, LET, 3);
                     }
                     if(lexer_peek(lexer,1) == 'o')if(lexer_peek(lexer,2) == 'o')if(lexer_peek(lexer,3) == 'p')if(ispeeker(lexer_peek(lexer,4))){
                         lexer_eat_arena(lexer, mem, LOOP, 4);
                     }
                 }break;
        case 'n':{
                     if(lexer_peek(lexer,1) == 'e')if(lexer_peek(lexer,2) == 'w')if(ispeeker(lexer_peek(lexer,3))){
                         lexer_eat_arena(lexer, mem, NEW, 3);
                     }
                     if(lexer_peek(lexer,1) == 'o')if(lexer_peek(lexer,2) == 't')if(ispeeker(lexer_peek(lexer,3))){
                         lexer_eat_arena(lexer, mem, NOT, 3);
                     }
                 }break;
        case 'e':{
                     if(lexer_peek(lexer,1) == 'l')if(lexer_peek(lexer,2) == 's')if(lexer_peek(lexer,3) == 'e')
                         if(ispeeker(lexer_peek(lexer,4))){
                             lexer_eat_arena(lexer, mem, ELSE, 4);
                         }
                     if(lexer_peek(lexer,1) == 's')if(lexer_peek(lexer,2) == 'a')if(lexer_peek(lexer,3) == 'c')if(ispeeker(lexer_peek(lexer,4))){
                         lexer_eat_arena(lexer, mem, ESAC, 4);
                     }
                 }break;
        case 'f':{
                     if(lexer_peek(lexer,1) == 'i')if(ispeeker(lexer_peek(lexer,2))){
                         lexer_eat_arena(lexer, mem, FI, 2);
                     }
                     if(lexer_peek(lexer,1) == 'a' || lexer_peek(lexer,1) == 'A' )
                         if(lexer_peek(lexer,2) == 'l' || lexer_peek(lexer,2) == 'L' )
                             if(lexer_peek(lexer,3) == 's' || lexer_peek(lexer,3) == 'S')
                                 if(lexer_peek(lexer,4) == 'e' || lexer_peek(lexer,4) == 'E')
                                     if(ispeeker(lexer_peek(lexer,5))){
                                         lexer_eat_arena(lexer, mem, FALSE, 5);
                                     }
                 }break;
        case 't':{
                     if(lexer_peek(lexer,1) == 'h')if(lexer_peek(lexer,2) == 'e')if(lexer_peek(lexer,3) == 'n')
                         if(ispeeker(lexer_peek(lexer,4))){
                             lexer_eat_arena(lexer, mem, THEN, 4);
                         }
                     if(lexer_peek(lexer,1) == 'r' || lexer_peek(lexer,1) == 'R' )if(lexer_peek(lexer,2) == 'u' || lexer_peek(lexer,2) == 'U' )if(lexer_peek(lexer,3) == 'e' || lexer_peek(lexer,3) == 'E')
                         if(ispeeker(lexer_peek(lexer,4))){
                             lexer_eat_arena(lexer, mem, TRUE, 4);
                         }
                 }break;
        case 'o':{
                     if(lexer_peek(lexer,1) == 'f')if(ispeeker(lexer_peek(lexer,2))){
                         lexer_eat_arena(lexer, mem, OF, 2);
                     }
                 }break;
        case 'p':{
                     if(lexer_peek(lexer,1) == 'o')if(lexer_peek(lexer,2) == 'o')if(lexer_peek(lexer,3) == 'l')
                         if(ispeeker(lexer_peek(lexer,4))){
                             lexer_eat_arena(lexer, mem, POOL, 4);
                         }
                 }break;
        case 'w':{
                     if(lexer_peek(lexer,1) == 'h')if(lexer_peek(lexer,2) == 'i')if(lexer_peek(lexer,3) == 'l')
                         if(lexer_peek(lexer,4) == 'e')if(ispeeker(lexer_peek(lexer,5))){
                             lexer_eat_arena(lexer, mem, WHILE, 5);
                         }
                 }break;
    } }

void lexer_eat_comments(lexer_t *lexer){
    int nested = 0;
    if(lexer->c == '-' && lexer_peek(lexer, 1) == '-') while(lexer->c != '\n') lexer_eat_n(lexer,1);
    if(lexer->c == '(' && lexer_peek(lexer, 1) == '*'){
        lexer_eat_n(lexer, 2);
        nested++;
        while(nested != 0){
            lexer_eat_n(lexer, 1);
            if(lexer->c == '(' && lexer_peek(lexer, 1) == '*') nested++;
            if(lexer->c == '*' && lexer_peek(lexer, 1) == ')') nested--;
        }
        lexer_eat_n(lexer, 2);
    }
}

void lexer_eat_whitespace(lexer_t *lexer){
    while(isspace(lexer->c)){
        lexer_eat_n(lexer, 1);
    }
}

void lexer_eat_integer(lexer_t *lexer, memory_t *mem){ 
    if(isdigit(lexer->c)){
        mem->tokens[mem->arena_i++] = INTEGER;
        while(isdigit(lexer->c)){    
            mem->tokens[mem->arena_i++] = lexer->c;
            lexer_eat_n(lexer, 1);
        } 
        mem->tokens[mem->arena_i++] = '\0';
    }
}

void lexer_eat_string(lexer_t *lexer, memory_t *mem){
    if(lexer->c == '"'){
        mem->tokens[mem->arena_i++] = LITERAL;
        lexer_eat_n(lexer, 1);
        while(lexer->c != '"'){
            mem->tokens[mem->arena_i++] = lexer->c;
            lexer_eat_n(lexer, 1);
        }
        mem->tokens[mem->arena_i++] = '\0';
        lexer_eat_n(lexer, 1);  
    }
}

void lexer_eat_type(lexer_t *lexer, memory_t *mem){
    if(isupper(lexer->c)){
        mem->tokens[mem->arena_i++] = TYPE_ID;
        while(isalnum(lexer->c) || lexer->c == '_'){
            mem->tokens[mem->arena_i++] = lexer->c;
            lexer_eat_n(lexer, 1);

        }
        mem->tokens[mem->arena_i++] = '\0';    
    } 
}

void lexer_eat_object(lexer_t *lexer, memory_t *mem){
    lexer_eat_keyword(lexer, mem);
    if(islower(lexer->c)){
        mem->tokens[mem->arena_i++] = OBJECT_ID;
        while(isalnum(lexer->c) || lexer->c == '_'){
            mem->tokens[mem->arena_i++] = lexer->c;
            lexer_eat_n(lexer, 1);

        }
        mem->tokens[mem->arena_i++] = '\0';    
    } 
}

void lexer_eat_punct(lexer_t *lexer, memory_t *mem){
    switch(lexer->c){
        case '+':{
                     lexer_eat_arena(lexer, mem, PLUS, 1);
                 }break;
        case '*':{
                     lexer_eat_arena(lexer, mem, STAR, 1);
                 }break;
        case '(':{
                     lexer_eat_arena(lexer, mem, LPAREN, 1);
                 }break;
        case ')':{
                     lexer_eat_arena(lexer, mem, RPAREN, 1);
                 }break;
        case '{':{
                     lexer_eat_arena(lexer, mem, LBRAK, 1);
                 }break;
        case '}':{
                     lexer_eat_arena(lexer, mem, RBRAK, 1);
                 }break;
        case '-':{
                     lexer_eat_arena(lexer, mem, MINUS, 1);
                 }break;
        case '~':{
                     lexer_eat_arena(lexer, mem, TILDE, 1);
                 }break;
        case '/':{
                     lexer_eat_arena(lexer, mem, SLASH, 1);
                 }break;
        case '=':{
                     if(lexer_peek(lexer,1) == '>'){
                         lexer_eat_arena(lexer, mem, MAP, 2);
                     }
                     lexer_eat_arena(lexer, mem, EQUAL, 1);
                 }break;
        case ';':{
                     lexer_eat_arena(lexer, mem, SEMICOLON, 1);
                 }break;
        case '.':{
                     lexer_eat_arena(lexer, mem, DOT, 1);
                 }break;
        case ':':{
                     lexer_eat_arena(lexer, mem, COLON, 1);
                 }break;
        case ',':{
                     lexer_eat_arena(lexer, mem, COMMA, 1);
                 }break;
        case '@':{
                     lexer_eat_arena(lexer, mem, ARROBA, 1);
                 }break;
        case '<':{
                     if(lexer_peek(lexer,1) == '='){
                         lexer_eat_arena(lexer, mem, LESSEQ, 2);
                     }
                     if(lexer_peek(lexer,1) == '-'){
                         lexer_eat_arena(lexer, mem, ASSIGN, 2);
                     }
                     else{
                         lexer_eat_arena(lexer, mem, LESS, 1);
                     }
                 }break;
    } 
}

void lexer_scan(lexer_t *lexer, memory_t *mem){    
    while(lexer->c != '\0'){
        /* printf("%c", lexer_peek(lexer, 0)); */
        lexer_eat_whitespace(lexer);
        lexer_eat_comments(lexer);
        if(ispunct(lexer->c))lexer_eat_punct(lexer, mem);
        lexer_eat_string(lexer, mem);
        lexer_eat_type(lexer, mem);
        lexer_eat_object(lexer, mem);
        lexer_eat_integer(lexer, mem);
    }
}








