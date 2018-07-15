#include <stdlib.h>
#include <stdio.h>

#include "argument.h"
#include "eval.h"
#include "lex.h"
#include "platform.h"
#include "token.h"
#include "utils.h"

ParsingResult * eval(char *input)
{
    ParsingResult * pr = parsing_result_new();
    if (!pr) {
        return pr;
    }

    ObjLex *oLex = lex_new(input);
    if (!oLex) {
        return NULL;
    }
    
    Token *tn;
    
    tn = lex_next(oLex);
    if (!tn) {
        goto LEX_FREE;
    }
    
    if (token_type(tn) != TOKEN_INT) {
        char *ss = space_get(token_loc(tn));

        fprintf(stderr, "%s%s", input, SEP);
        fprintf(stderr, "%s^ -- invalid string at %u: %s%s", 
            ss, token_loc(tn), token_str(tn), SEP);
        
        if (token_loc(tn) > 0) {
            free(ss);
        }
    }

LEX_FREE:
    lex_free(oLex);
    
    return pr;
}