#include "../include/parser.h"
#include <stdio.h>

parser_t *parser_init(memory_t *mem){
    parser_t *parser = calloc(1, sizeof(parser_t));
    parser->stream = mem;
    parser->mem_i = 0;

    return parser;
}

u1 parser_must_exit(parser_t *parser, u1 expected){
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
// ID( [ expr [, expr]* ] )
void parser_ast_exp_fun(parser_t *parser, struct tree *tree){ 
    if(parser_peek_exit(parser, COMMA)){
        parser_must_exit(parser, COMMA);
        tree = tree->right_value;
        parser_ast_exp(parser, tree);
    }

    parser_ast_exp(parser, tree);
}

void parser_ast_exp_terminal(parser_t *parser, struct tree *tree){
    if(parser_peek_exit(parser, LITERAL)){
        parser_must_exit(parser, LITERAL);
        tree->right_value = tree_init_ID(LITERAL, parser->mem_i);
        while(parser->stream->tokens[parser->mem_i-1] != '\0') parser->mem_i++; 
    }else if(parser_peek_exit(parser, INTEGER)){
        parser_must_exit(parser, INTEGER);
        tree->right_value = tree_init_ID(INTEGER, parser->mem_i);
        while(parser->stream->tokens[parser->mem_i-1] != '\0') parser->mem_i++; 
    }else if(parser_peek_exit(parser, TRUE)){
        parser_must_exit(parser, TRUE);
        tree->right_value = tree_init(TRUE);
    }else if(parser_peek_exit(parser, FALSE)){
        parser_must_exit(parser, FALSE);
        tree->right_value = tree_init(FALSE);
    }
}
// let ID : TYPE [ <- expr ] [, ID : TYPE [ <- expr ]]* in expr
void parser_ast_exp_let(parser_t *parser, struct tree *tree){
    if(parser_must_exit(parser, OBJECT_ID)){
        tree->right_value = tree_init_ID(OBJECT_ID, parser->mem_i);
        tree = tree->right_value;
        while(parser->stream->tokens[parser->mem_i-1] != '\0') parser->mem_i++; 
        if(parser_must_exit(parser, COLON)){
            if(parser_must_exit(parser, TYPE_ID)){ 
                tree->right_value = tree_init_ID(COLON, parser->mem_i);
                while(parser->stream->tokens[parser->mem_i-1] != '\0') parser->mem_i++; 
            }
            if(parser_peek_exit(parser, ASSIGN)){ 
                struct tree *temp = tree->right_value;
                parser_must_exit(parser, ASSIGN);
                temp->left_derive = tree_init(ASSIGN);
                temp = temp->left_derive;
                parser_ast_exp(parser, temp);
            }
        }
        if(parser_must_exit(parser, IN)){
            tree->left_derive = tree_init(IN);
            tree = tree->left_derive;
            parser_ast_exp(parser, tree);
        }
    }
}

void parser_ast_dot(parser_t *parser, struct tree *tree){ 
        parser_must_exit(parser, DOT);
        if(parser_peek_exit(parser, OBJECT_ID)){
            parser_must_exit(parser, OBJECT_ID);
            tree->left_derive = tree_init_ID(OBJECT_ID, parser->mem_i);
            tree = tree->left_derive;
            while(parser->stream->tokens[parser->mem_i-1] != '\0') parser->mem_i++; 
            struct tree *temp1 = tree;
            if(parser_peek_exit(parser, LPAREN)){
                parser_must_exit(parser, LPAREN);
                temp1->right_value = tree_init(LPAREN);
                temp1 = temp1->right_value;

                parser_ast_exp_fun(parser, temp1);

                parser_must_exit(parser, RPAREN);
                temp1->left_derive = tree_init(RPAREN);
            }
        }
}

void parser_ast_exp(parser_t *parser, struct tree *tree){
    parser_ast_exp_terminal(parser, tree);
    if(parser_peek_exit(parser, LBRAK))parser_must_exit(parser, LBRAK);
    if(parser_peek_exit(parser, RBRAK))parser_must_exit(parser, RBRAK); 
    if(parser_peek_exit(parser, OBJECT_ID)){
        parser_must_exit(parser, OBJECT_ID);
        tree->left_derive = tree_init_ID(OBJECT_ID, parser->mem_i);
        tree = tree->left_derive;
        while(parser->stream->tokens[parser->mem_i-1] != '\0') parser->mem_i++; 
        struct tree *temp1 = tree;
        if(parser_peek_exit(parser, LPAREN)){
            parser_must_exit(parser, LPAREN);
            temp1->right_value = tree_init(LPAREN);
            temp1 = temp1->right_value;

            parser_ast_exp_fun(parser, temp1);

            parser_must_exit(parser, RPAREN);
            temp1->left_derive = tree_init(RPAREN);
        }
        if(parser_peek_exit(parser, SEMICOLON)){
            parser_must_exit(parser, SEMICOLON);
            parser_ast_exp(parser, tree);
        }
    }
    if(parser_peek_exit(parser, PLUS) || parser_peek_exit(parser, LESS) || parser_peek_exit(parser, MINUS) || parser_peek_exit(parser, STAR) || parser_peek_exit(parser, SLASH) || parser_peek_exit(parser, EQUAL) || parser_peek_exit(parser, LESSEQ) ){
        tree->right_value = tree_init(parser->stream->tokens[parser->mem_i]);
        parser->mem_i += 3;
        struct tree *temp = tree->right_value;
        parser_ast_exp(parser, temp);
    }

    if(parser_peek_exit(parser, LET)){
        parser_must_exit(parser, LET);
        tree->left_derive = tree_init(LET);
        tree = tree->left_derive;
        struct tree *temp2 = tree;
        parser_ast_exp_let(parser, temp2);
        if(parser_peek_exit(parser, SEMICOLON)){
            parser_must_exit(parser, SEMICOLON);
            parser_ast_exp(parser, tree);
        }
    }
    // if expr then expr else expr fi
    if(parser_peek_exit(parser, IF)){
        parser_must_exit(parser, IF);
        tree->left_derive = tree_init(IF);
        tree = tree->left_derive;
        parser_ast_exp(parser, tree);
        if(parser_must_exit(parser, THEN)){
            tree->right_value = tree_init(THEN);
            tree = tree->right_value;
            parser_ast_exp(parser, tree);
            if(parser_must_exit(parser, ELSE)){
                tree->right_value = tree_init(ELSE);
                tree = tree->right_value;
                parser_ast_exp(parser, tree);
                if(parser_must_exit(parser, FI)){
                    tree->right_value = tree_init(FI);
                    tree = tree->right_value;
                }
            }
        }
    }
}

// formal ::= ID : TYPE
void parser_ast_formal(parser_t *parser, struct tree *tree){ 
    if(parser_must_exit(parser, OBJECT_ID)){ 
        tree->left_derive = tree_init_ID(OBJECT_ID, parser->mem_i);
        while(parser->stream->tokens[parser->mem_i-1] != '\0') parser->mem_i++; 
        tree = tree->left_derive;
        if(parser_must_exit(parser, COLON)){
            if(parser_must_exit(parser, TYPE_ID)){ 
                tree->right_value = tree_init_ID(COLON, parser->mem_i);
                while(parser->stream->tokens[parser->mem_i-1] != '\0') parser->mem_i++; 
                if(parser_peek_exit(parser, RPAREN)){
                    parser_must_exit(parser, RPAREN);
                    tree->left_derive = tree_init(RPAREN); 
                } 
                if(parser_peek_exit(parser, COMMA)){
                    parser_must_exit(parser, COMMA);
                    parser_ast_formal(parser, tree);
                } 
            }
        }
    }
}

//  feature ->  ID( [ formal [, formal]* ] ) : TYPE { expr }
//          |   ID : TYPE [ <- expr ]
void parser_ast_feature(parser_t *parser, struct tree *tree){ 
    if(parser_peek_exit(parser, OBJECT_ID)){
        parser_must_exit(parser, OBJECT_ID);
        tree->left_derive = tree_init_ID(LBRAK, parser->mem_i);
        while(parser->stream->tokens[parser->mem_i-1] != '\0') parser->mem_i++; 
        tree = tree->left_derive;
    }
    if(parser_peek_exit(parser, LPAREN)){
        parser_must_exit(parser, LPAREN);
        tree->right_value = tree_init(LPAREN);
        tree = tree->right_value;
        if(parser_peek_exit(parser, RPAREN)){
            if(parser_must_exit(parser, RPAREN)) tree->left_derive = tree_init(RPAREN); 
        }else parser_ast_formal(parser, tree);
        if(parser_must_exit(parser, COLON)){     
            if(parser_must_exit(parser, TYPE_ID)){
                tree->right_value = tree_init(COLON);
                tree = tree->right_value;
                tree->right_value = tree_init_ID(TYPE_ID, parser->mem_i);
                while(parser->stream->tokens[parser->mem_i-1] != '\0') parser->mem_i++; 
                if(parser_must_exit(parser, LBRAK)){
                    tree->left_derive = tree_init(LBRAK);
                    tree = tree->left_derive;
                    parser_ast_exp(parser, tree);
                }
            }
        }
    }else if(parser_must_exit(parser, COLON)){
        if(parser_must_exit(parser, TYPE_ID)){
            tree->right_value = tree_init_ID(COLON, parser->mem_i);
            while(parser->stream->tokens[parser->mem_i-1] != '\0') parser->mem_i++; 
            if(parser_must_exit(parser, ASSIGN)){
                tree->right_value = tree_init(ASSIGN);
                tree = tree->right_value;
                parser_ast_exp(parser, tree);
            }
        }
    }
    if(parser_must_exit(parser, RBRAK)){
        tree->right_value = tree_init(RBRAK);
    }
}

// class ::= class TYPE (* [inherits TYPE] { [feature; ]* } *)
void parser_ast_class(parser_t *parser, struct tree *tree){
    // INHERITS type goes to the right of the tree
    if(parser_must_exit(parser, INHERITS)){
        if(parser_must_exit(parser, TYPE_ID)){
            tree->right_value = tree_init_ID(INHERITS, parser->mem_i);
            while(parser->stream->tokens[parser->mem_i-1] != '\0') parser->mem_i++; 
        }
    }
    // 'feature' derives to the left
    if(parser_must_exit(parser, LBRAK)){ 
        //  feature ->  ID( [ formal [, formal]* ] ) : TYPE { expr }
        //          |   ID : TYPE [ <- expr ]
        parser_ast_feature(parser, tree);    
        if(parser_must_exit(parser, SEMICOLON)){
            if(!parser_peek_exit(parser, RBRAK)){
                tree = tree->left_derive;
                parser_ast_feature(parser, tree);
            }
        }
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
        // class ::= (* class TYPE *)[inherits TYPE] { [[feature; ]]* }
        if(parser_peek_exit(parser, CLASS)){
            parser_must_exit(parser, CLASS);
            if(parser_must_exit(parser, TYPE_ID)){
                // start first class root
                (*ast)->classe[x] = tree_init_ID(CLASS, parser->mem_i);
                while(parser->stream->tokens[parser->mem_i-1] != '\0') parser->mem_i++; 
                parser_ast_class(parser, (*ast)->classe[x]);
                ++x;
            }
        }else parser->mem_i++;
    }
}

