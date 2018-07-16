#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "argument.h"
#include "eval.h"
#include "lex.h"
#include "platform.h"
#include "token.h"
#include "utils.h"

static void show_error(char *input, Token *tn);

// eval is the combo of lexer, parser, and interpreter.
// We mixed parser and interpreter due to the simple and fixed format of d20 string.
ParsingResult * eval(char *input)
{
    ParsingResult * pr = parsing_result_new();
    if (!pr) {
        return pr;
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "No valid d20 string%s", SEP);

        parsing_result_free(pr);
        return NULL;
    }

    ObjLex *oLex = lex_new(input);
    if (!oLex) {
        return NULL;
    }
    
    Token *tn;

    // 1st token is mandatory.
    tn = lex_next(oLex);
    if (!tn) {
        goto PARSE_FAIL;
    }

    // 1st token should be TOKEN_INT, e.g. 1d6.    
    if (token_type(tn) != TOKEN_INT) {
        show_error(input, tn);
        goto PARSE_FAIL;
    }
    
    // 2nd token is mandatory.
    tn = lex_next(oLex);
    if (!tn) {
        goto PARSE_FAIL;
    }
    
    // 2nd token should be TOKEN_DICE, e.g. 1d6.
    if (token_type(tn) != TOKEN_DICE) {
        show_error(input, tn);
        goto PARSE_FAIL;
    }
    
    // 3rd token is mandatory.
    tn = lex_next(oLex);
    if (!tn) {
        goto PARSE_FAIL;
    }
    
    // 3rd token should be TOKEN_INT, e.g. 1d6.
    if (token_type(tn) != TOKEN_INT) {
        show_error(input, tn);
        goto PARSE_FAIL;
    }
    
    // 4th token is optional.
    tn = lex_next(oLex);
    if (!tn) {
        goto PARSE_END;
    }
    
    // 4th token should be TOKEN_SIGN, e.g. 1d6+2.
    if (token_type(tn) != TOKEN_SIGN) {
        show_error(input, tn);
        goto PARSE_FAIL;
    }
    
    // 5th token is mandatory when 4th token exists.
    tn = lex_next(oLex);
    if (!tn) {
        char *ss = space_get(strlen(input));
        
        fprintf(stderr, "%s%s", input, SEP);
        fprintf(stderr, "%s^ -- no valid number at %u%s", 
            ss, strlen(input) + 1, SEP);

        if (strlen(ss) > 0) {
            free(ss);
        }

        goto PARSE_FAIL;
    }
    
    // 5th token should be TOKEN_INT, e.g. 1d6+2.
    if (token_type(tn) != TOKEN_INT) {
        show_error(input, tn);
        goto PARSE_FAIL;
    }

PARSE_END:
    return pr;

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
