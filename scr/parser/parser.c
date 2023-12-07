#include "../include/parser.h"
#include <stdio.h>
#include <threads.h>
#define DEBUG 1

parser_t *parser_init(memory_t *mem){
    parser_t *parser = calloc(1, sizeof(parser_t));
    parser->stream = mem;
    parser->mem_i = 0;

    return parser;
}

u1 parser_eat_exit(parser_t *parser, u1 expected){
    if(parser->stream->tokens[parser->mem_i] != expected){
        printf("Syntax Error: Expected '");
        token_print_string(expected, stdout);
        printf("'found '");
        token_print_string(parser->stream->tokens[parser->mem_i], stdout);
        printf("' at %i|%i\n", parser->stream->tokens[parser->mem_i+1], parser->stream->tokens[parser->mem_i+2]);
        exit(1);
    }

    parser->mem_i += 3;
    return 1;
}
u1 parser_peek_exit(parser_t *parser, u1 expected){
    if(parser->stream->tokens[parser->mem_i] != expected) return 0;

    return 1;
}
// formal ::= ID : TYPE
struct tree *parser_ast_formal(parser_t *parser, struct tree *tree){ 
    if(parser_eat_exit(parser, OBJECT_ID)){ 
        tree->left_derive = tree_init_ID(OBJECT_ID, parser->mem_i);
        while(parser->stream->tokens[parser->mem_i-1] != '\0') parser->mem_i++; 
        tree = tree->left_derive;
        if(parser_eat_exit(parser, COLON)){
            if(parser_eat_exit(parser, TYPE_ID)){ 
                tree->right_value = tree_init_ID(TYPE_ID, parser->mem_i);
                while(parser->stream->tokens[parser->mem_i-1] != '\0') parser->mem_i++; 
            }
        }
    }

    return tree;
}
//  feature ->  ID( [ formal [, formal]* ] ) : TYPE { expr } 
//          |   ID : TYPE [ <- expr ](must do this one)
void parser_ast_feature(parser_t *parser, struct tree *tree){ 
    if(parser_eat_exit(parser, OBJECT_ID)){ 
        tree->right_value = tree_init_ID(OBJECT_ID, parser->mem_i);
        while(parser->stream->tokens[parser->mem_i-1] != '\0') parser->mem_i++; 
        tree = tree->right_value;
        if(parser_peek_exit(parser, LPAREN)){
            parser_eat_exit(parser, LPAREN);
            tree->left_derive = tree_init(LPAREN);
            struct tree *formal = tree->left_derive;
            while(!parser_peek_exit(parser, RPAREN)){
                if(parser_peek_exit(parser, COMMA)){
                    parser_eat_exit(parser, COMMA);
                }
                formal = parser_ast_formal(parser, formal);
            }
            if(parser_eat_exit(parser, RPAREN)) formal->left_derive = tree_init(RPAREN);
        }
        if(parser_eat_exit(parser, COLON)){
            if(parser_eat_exit(parser, TYPE_ID)){
                tree->right_value = tree_init_ID(TYPE_ID, parser->mem_i);
                while(parser->stream->tokens[parser->mem_i-1] != '\0') parser->mem_i++; 
                tree = tree->right_value;
                if(parser_peek_exit(parser, LBRAK)){
                    parser_eat_exit(parser, LBRAK);
                    tree->right_value = tree_init(LBRAK);
                    tree->right_value->right_value = tree_init(EXPRESSION);
                    tree = tree->right_value->right_value;
                    struct tree *root = tree;
                    tree = parser_ast_exps(parser, tree);
                    if(parser_eat_exit(parser, RBRAK)) root->right_value = tree_init(RBRAK);
                }else if(parser_peek_exit(parser, ASSIGN)){
                    parser_eat_exit(parser, ASSIGN);
                    tree->right_value = tree_init(ASSIGN);
                    tree = tree->right_value;
                    while(!parser_peek_exit(parser, SEMICOLON)){
                        tree = parser_ast_exps(parser, tree);
                        tree = tree->right_value;
                    }
                }
            }
        }
    }
}

// class ::= class TYPE (* [inherits TYPE] { [feature; ]* } *) // features loop working
void parser_ast_class(parser_t *parser, struct tree *tree){
    // INHERITS type goes to the right of the tree
    if(parser_peek_exit(parser, INHERITS)){
        parser_eat_exit(parser, INHERITS);
        if(parser_eat_exit(parser, TYPE_ID)){
            tree->right_value = tree_init_ID(TYPE_ID, parser->mem_i);
            while(parser->stream->tokens[parser->mem_i-1] != '\0') parser->mem_i++; 
        }
    }
    // 'feature' derives to the left
    if(parser_eat_exit(parser, LBRAK)){ 
        tree->left_derive = tree_init(LBRAK);
        tree = tree->left_derive;
        while(!parser_peek_exit(parser, RBRAK)){
            tree->left_derive = tree_init(FEATURE);
            tree = tree->left_derive;
            parser_ast_feature(parser, tree);
            parser_eat_exit(parser, SEMICOLON);
        }
        if(parser_eat_exit(parser, RBRAK)) tree->left_derive = tree_init(RBRAK);
    }
}

void parser_free(parser_t *parser){
    free(parser->stream);
    free(parser);
}

// rewrite with all tokens
// each one of them exist for a reason
void parser_parse(parser_t *parser, ast_t **ast){
    int x = 0;
    while(parser->mem_i < parser->stream->arena_i){
        // class ::= (* class TYPE *)[inherits TYPE] { [feature; ]* }
        if(parser_eat_exit(parser, CLASS)){
            if(parser_eat_exit(parser, TYPE_ID)){
                // start first class root
                (*ast)->classe[x] = tree_init_ID(TYPE_ID, parser->mem_i);
                while(parser->stream->tokens[parser->mem_i-1] != '\0') parser->mem_i++; 
                parser_ast_class(parser, (*ast)->classe[x]);
            }
            if(parser_eat_exit(parser, SEMICOLON)) ++x;
        }
    }
}

/* 
   ID
   ID( [ expr [, expr]* ] )
   ID <- expr
   */
struct tree *parser_ast_exp_struct(parser_t *parser, struct tree *tree){
    if(parser_peek_exit(parser, OBJECT_ID)){
        tree->right_value = tree_init_ID(OBJECT_ID, parser->mem_i);
        while(parser->stream->tokens[parser->mem_i-1] != '\0') parser->mem_i++; 
    }

    return tree;
} /*
   expr + expr
   expr − expr
   expr * expr
   expr / expr
   expr < expr
   expr <= expr
   expr = expr
   */
struct tree *parser_ast_exp_operation(parser_t *parser, struct tree *tree){
    char flag = 0;
    if(parser_peek_exit(parser, PLUS)){
        parser_eat_exit(parser, PLUS);
        tree->right_value = tree_init(PLUS);
        tree = tree->right_value;
        flag = 1;
    }else if(parser_peek_exit(parser, STAR)){
        parser_eat_exit(parser, STAR);
        tree->right_value = tree_init(STAR);
        tree = tree->right_value;
        flag = 1;
    }else if(parser_peek_exit(parser, MINUS)){
        parser_eat_exit(parser, MINUS);
        tree->right_value = tree_init(MINUS);
        tree = tree->right_value;
        flag = 1;
    }else if(parser_peek_exit(parser, SLASH)){
        parser_eat_exit(parser, SLASH);
        tree->right_value = tree_init(SLASH);
        tree = tree->right_value;
        flag = 1;
    }else if(parser_peek_exit(parser, LESS)){
        parser_eat_exit(parser, LESS);
        tree->right_value = tree_init(LESS);
        tree = tree->right_value;
        flag = 1;
    }else if(parser_peek_exit(parser, LESSEQ)){
        parser_eat_exit(parser, LESSEQ);
        tree->right_value = tree_init(LESSEQ);
        tree = tree->right_value;
        flag = 1;
    }else if(parser_peek_exit(parser, EQUAL)){
        parser_eat_exit(parser, EQUAL);
        tree->right_value = tree_init(EQUAL);
        tree = tree->right_value;
        flag = 1;
    }

    if(parser_peek_exit(parser, LESS) || parser_peek_exit(parser, EQUAL) || parser_peek_exit(parser, LESSEQ) || parser_peek_exit(parser, SLASH) || parser_peek_exit(parser, MINUS) || parser_peek_exit(parser, PLUS) || parser_peek_exit(parser, STAR)){
        printf("Syntax Error: Operators should not follow another operator\n");
        exit(1);
    }
    
    if(flag) return parser_ast_exps(parser, tree);

    return tree;
}
/*
   not expr
   ˜expr
   */
struct tree *parser_ast_exp_punct(parser_t *parser, struct tree *tree){
    if(parser_peek_exit(parser, NOT)){
        parser_eat_exit(parser, NOT);
        tree->left_derive = tree_init(NOT);
        tree = tree->left_derive;

        tree = parser_ast_exps(parser, tree);
    }else if(parser_peek_exit(parser, TILDE)){
        parser_eat_exit(parser, TILDE);
        tree->left_derive = tree_init(TILDE);
        tree = tree->left_derive;

        tree = parser_ast_exps(parser, tree);
    }else if(parser_peek_exit(parser, LPAREN)){
        parser_eat_exit(parser, LPAREN);
        tree->left_derive = tree_init(LPAREN);
        tree = tree->left_derive;

        return parser_ast_exps(parser, tree);
    }

    return tree;
}
/*
   integer
   string
   true
   false
   */
struct tree *parser_ast_exp_terminal(parser_t *parser, struct tree *tree){ 
    if(parser_peek_exit(parser, INTEGER)){
        parser_eat_exit(parser, INTEGER);
        tree->left_derive = tree_init_ID(INTEGER, parser->mem_i);
        while(parser->stream->tokens[parser->mem_i-1] != '\0') parser->mem_i++; 
        tree = tree->left_derive;
    }else if(parser_peek_exit(parser, LITERAL)){
        parser_eat_exit(parser, LITERAL);
        tree->left_derive = tree_init_ID(LITERAL, parser->mem_i); 
        while(parser->stream->tokens[parser->mem_i-1] != '\0') parser->mem_i++; 
        tree = tree->left_derive;
    }else if(parser_peek_exit(parser, TRUE)){
        parser_eat_exit(parser, TRUE);
        tree->left_derive = tree_init(TRUE);
        tree = tree->left_derive;
    }else if(parser_peek_exit(parser, FALSE)){ 
        parser_eat_exit(parser, FALSE);
        tree->left_derive = tree_init(FALSE);
        tree = tree->left_derive;
    }

    return tree;
}
/*
   expr ::= expr[@TYPE].ID( [ expr [[, expr]]∗ ] )
   while expr loop expr pool
   case expr of [[ID : TYPE => expr; ]]+ esac
   new TYPE
   isvoid expr
   */
struct tree *parser_ast_exps(parser_t *parser, struct tree *tree){
    
    /*
        { [expr; ]+ } <- DO THIS ONE
    */
    if(parser_peek_exit(parser, LBRAK)){
        parser_eat_exit(parser, LBRAK);
        struct tree *root = tree; 
        tree = parser_ast_exps(parser, tree);
        parser_eat_exit(parser, SEMICOLON);
        parser_eat_exit(parser, RBRAK);
    }
    if(parser_peek_exit(parser, RBRAK) || parser_peek_exit(parser, SEMICOLON)) return tree;

    tree = parser_ast_exp_operation(parser, tree);
    tree = parser_ast_exp_terminal(parser, tree);
    tree = parser_ast_exp_punct(parser, tree);
 
    return parser_ast_exps(parser, tree);
}


// if expr then expr else expr fi
void parser_ast_exp_if(parser_t *parser, struct tree *tree){
}

// let ID : TYPE [ <- expr ] [, ID : TYPE [ <- expr ]]* in expr
void parser_ast_exp_let(parser_t *parser, struct tree *tree){
}









