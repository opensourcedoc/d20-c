#ifndef D20_TOKEN_H
#define D20_TOKEN_H

typedef enum {
    TOKEN_INT,
    TOKEN_DICE,
    TOKEN_SIGN
} TOKEN_TYPE;

typedef struct token Token;

Token * token_new(char *s, TOKEN_TYPE t);
void token_free(void *self);
char * token_str(Token *self);
TOKEN_TYPE token_type(Token *self);

#endif  // D20_TOKEN_H