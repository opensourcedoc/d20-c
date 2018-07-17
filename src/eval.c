#include <stdbool.h>
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
bool eval(char *input, ParsingResult *out)
{
    if (strlen(input) == 0) {
        fprintf(stderr, "No valid d20 string%s", SEP);
        return false;
    }

    ObjLex *oLex = lex_new(input);
    if (!oLex) {
        return false;
    }
    
    Token *tn;
    char *str_r;
    char *str_d;
    char *str_sign;
    char *str_m;

    // 1st token is mandatory.
    tn = lex_next(oLex);
    if (!tn) {
        char *ss = space_get(strlen(input));
        
        fprintf(stderr, "%s%s", input, SEP);
        fprintf(stderr, "%s^ -- no valid number at %u%s", 
            ss, (unsigned) strlen(input) + 1, SEP);

        if (strlen(ss) > 0) {
            free(ss);
        }
        
        goto PARSE_FAIL;
    }

    // 1st token should be TOKEN_INT, e.g. 1d6.    
    if (token_type(tn) != TOKEN_INT) {
        show_error(input, tn);
        goto PARSE_FAIL;
    }
    
    str_r = token_str(tn);
    
    // 2nd token is mandatory.
    tn = lex_next(oLex);
    if (!tn) {
        char *ss = space_get(strlen(input));
        
        fprintf(stderr, "%s%s", input, SEP);
        fprintf(stderr, "%s^ -- no valid dice string at %u%s", 
            ss, (unsigned) strlen(input) + 1, SEP);

        if (strlen(ss) > 0) {
            free(ss);
        }
        
        goto PARSE_FAIL;
    }
    
    // 2nd token should be TOKEN_DICE, e.g. 1d6.
    if (token_type(tn) != TOKEN_DICE) {
        show_error(input, tn);
        goto PARSE_FAIL;
    }
    
    // Discard 'd' (dice string).
    
    // 3rd token is mandatory.
    tn = lex_next(oLex);
    if (!tn) {
        char *ss = space_get(strlen(input));
        
        fprintf(stderr, "%s%s", input, SEP);
        fprintf(stderr, "%s^ -- no valid dice face at %u%s", 
            ss, (unsigned) strlen(input) + 1, SEP);

        if (strlen(ss) > 0) {
            free(ss);
        }
        
        goto PARSE_FAIL;
    }

    // 3rd token should be TOKEN_INT, e.g. 1d6.
    if (token_type(tn) != TOKEN_INT) {
        show_error(input, tn);
        goto PARSE_FAIL;
    }
    
    str_d = token_str(tn);
    
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
    
    str_sign = token_str(tn);
    
    // 5th token is mandatory when 4th token exists.
    tn = lex_next(oLex);
    if (!tn) {
        char *ss = space_get(strlen(input));
        
        fprintf(stderr, "%s%s", input, SEP);
        fprintf(stderr, "%s^ -- no valid number at %u%s", 
            ss, (unsigned) strlen(input) + 1, SEP);

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
    
    str_m = token_str(tn);
    
    // 6th or further token is invalid.
    tn = lex_next(oLex);
    if (tn) {
        show_error(input, tn);
        goto PARSE_FAIL;
    }

    size_t sz = strlen(str_sign) + strlen(str_m) + 1;
    char *ss = malloc(sz * sizeof(char));

#pragma warning(disable:4996)
    strcpy(ss, str_sign);
    strcat(ss, str_m);
    ss[sz] = '\0';

    int m = strtol(ss, NULL, 10);
    parsing_result_set_modifier(out, m);
    
    free(ss);

    unsigned r;
    unsigned d;
PARSE_END:    
    r = strtoul(str_r, NULL, 10);
    parsing_result_set_roll(out, r);
    
    d = strtoul(str_d, NULL, 10);
    parsing_result_set_dice(out, d);
    
    lex_free(oLex);

    return true;

PARSE_FAIL:
    lex_free(oLex);
    
    return false;
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
