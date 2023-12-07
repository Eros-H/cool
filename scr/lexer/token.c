#include "../include/token.h"

memory_t *memory_init(u64 size){
    memory_t *mem = calloc(1, sizeof(struct STRUCT_MEM));
    mem->tokens = calloc(size, sizeof(u8));
    mem->arena_i = 0;
    mem->arena_size = size;
    mem->class_size = 0;

    return mem;
}

u1 token_size(int type){
    switch (type) {
        case ID: return 2; break;
        case INTEGER: return 7; break;
        case TYPE_ID: return 7; break;
        case OBJECT_ID: return 9; break;
        case SELF_ID: return 7; break;
        case LITERAL: return 7; break;
        case BOOLEAN: return 7; break;
        case SELF:  return 4; break;
        case MAP:  return 3; break;
        case COMMA: return 5; break;
        case COLON: return 5; break;
        case SEMICOLON: return 9; break; 
        case EXPRESSION: return 10; break; 
        case LPAREN: return 6; break;
        case RPAREN: return 6; break;
        case LBRAK: return 4; break;
        case RBRAK: return 4; break;
        case EQUAL: return 4; break;
        case ASSIGN: return 5; break;
        case PLUS: return 4; break;
        case CLASS: return 5; break;
        case MINUS: return 5; break;
        case SLASH: return 5; break;
        case TILDE: return 4; break; 
        case DOT: return 3; break;
        case STAR: return 4; break;
        case IF: return 2; break;
        case FI: return 2; break;
        case ELSE: return 4; break;
        case THEN: return 4; break;
        case LESS: return 4; break;
        case LESSEQ: return 6; break;
        case FEATURE: return 7 ; break;
        case ARROBA: return 6; break;
        case LOOP: return 4; break;
        case POOL: return 4; break;
        case CASE: return 4; break;
        case ESAC: return 4; break;
        case WHILE: return 5; break;
        case LET: return 3; break;
        case NEW: return 3; break;
        case OF: return 2; break;
        case IN: return 2; break;
        case ISVOID: return 6; break;
        case INHERITS: return 8; break;
        case TRUE: return 4; break;
        case FALSE:  return 5; break;
        case NOT: return 3; break;
    }

    return 0;
}

void token_print_string(int type, FILE *f) {
    switch (type) {
        case ID: fprintf(f,"id"); break;
        case INTEGER: fprintf(f,"integer"); break;
        case TYPE_ID: fprintf(f,"type_ID"); break;
        case OBJECT_ID: fprintf(f,"object_ID"); break;
        case SELF_ID: fprintf(f,"self_ID"); break;
        case LITERAL: fprintf(f,"literal"); break;
        case BOOLEAN: fprintf(f,"boolean"); break;
        case SELF: fprintf(f,"self"); break;
        case MAP: fprintf(f,"map"); break;
        case COMMA: fprintf(f,"comma"); break;
        case COLON: fprintf(f,"colon"); break;
        case SEMICOLON: fprintf(f,"semicolon"); break;
        case LPAREN: fprintf(f,"lparen"); break;
        case RPAREN: fprintf(f,"rparen"); break;
        case LBRAK: fprintf(f,"lbrak"); break;
        case RBRAK: fprintf(f,"rbrak"); break;
        case EQUAL: fprintf(f,"equal"); break;
        case ASSIGN: fprintf(f,"assign"); break;
        case PLUS: fprintf(f,"plus"); break;
        case CLASS: fprintf(f,"class"); break;
        case MINUS: fprintf(f,"minus"); break;
        case SLASH: fprintf(f,"slash"); break;
        case TILDE: fprintf(f,"tilde"); break;
        case DOT: fprintf(f,"dot"); break;
        case STAR: fprintf(f,"star"); break;
        case IF: fprintf(f,"if"); break;
        case FI: fprintf(f,"fi"); break;
        case ELSE: fprintf(f,"else"); break;
        case THEN: fprintf(f,"then"); break;
        case LESS: fprintf(f,"less"); break;
        case LESSEQ: fprintf(f,"lesseq"); break;
        case ARROBA: fprintf(f,"arroba"); break;
        case LOOP: fprintf(f,"loop"); break;
        case POOL: fprintf(f,"pool"); break;
        case CASE: fprintf(f,"case"); break;
        case ESAC: fprintf(f,"esac"); break;
        case WHILE: fprintf(f,"while"); break;
        case LET: fprintf(f,"let"); break;
        case NEW: fprintf(f,"new"); break;
        case OF: fprintf(f,"of"); break;
        case IN: fprintf(f,"in"); break;
        case ISVOID: fprintf(f,"isvoid"); break;
        case INHERITS: fprintf(f,"inherits"); break;
        case EXPRESSION: fprintf(f,"expression"); break;
        case FEATURE: fprintf(f,"feature"); break;
        case TRUE: fprintf(f,"true"); break;
        case FALSE: fprintf(f,"false"); break;
        case NOT: fprintf(f,"not"); break;
        default: fprintf(f,"invalid");
    }
}
