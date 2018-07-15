#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "token_internal.h"

Token * token_new(char *s, TOKEN_TYPE t, unsigned loc)
{
    Token *token = (Token *) malloc(sizeof(Token));
    if (!token) {
        return token;
    }
    
    size_t sz = strlen(s) + 1;
    token->str = malloc(sz * sizeof(char));
    strcpy(token->str, s);
    token->str[sz] = '\0';

    token->t = t;
    token->loc = loc;
    
    return token;
}

void token_free(void *self)
{
    if (!self) {
        return;
    }

    char *s = ((Token *) self)->str;
    if (s) {
        free(s);
    }

    free(self);
}

char * token_str(Token *self)
{
    assert(self);
    
    return self->str;
}

TOKEN_TYPE token_type(Token *self)
{
    assert(self);
    
    return self->t;
}

unsigned token_loc(Token *self)
{
    assert(self);
    
    return self->loc;
}
