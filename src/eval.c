#include <stdlib.h>
#include <stdio.h>

#include "argument.h"
#include "eval.h"
#include "lex.h"
#include "platform.h"
#include "token.h"
#include "utils.h"

static void show_error(char *input, Token *tn);

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
        goto PARSE_FAIL;
    }
    
    if (token_type(tn) != TOKEN_INT) {
        show_error(input, tn);
        goto PARSE_FAIL;
    }
    
    tn = lex_next(oLex);
    if (!tn) {
        goto PARSE_FAIL;
    }
    
    if (token_type(tn) != TOKEN_DICE) {
        show_error(input, tn);
        goto PARSE_FAIL;
    }
    
    tn = lex_next(oLex);
    if (!tn) {
        goto PARSE_FAIL;
    }
    
    if (token_type(tn) != TOKEN_INT) {
        show_error(input, tn);
        goto PARSE_FAIL;
    }

PARSE_FAIL:
    lex_free(oLex);
    parsing_result_free(pr);
    
    return NULL;
}

static void show_error(char *input, Token *tn)
{
    char *ss = space_get(token_loc(tn));
        
    fprintf(stderr, "%s%s", input, SEP);
    fprintf(stderr, "%s^ -- invalid string at %u: %s%s", 
        ss, token_loc(tn) + 1, token_str(tn), SEP);

    if (strlen(ss) > 0) {
        free(ss);
    }
}