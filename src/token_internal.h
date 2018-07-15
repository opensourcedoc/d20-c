#ifndef D20_TOKEN_INTERNAL_H
#define D20_TOKEN_INTERNAL_H

struct token {
    char * str;
    TOKEN_TYPE t;
    unsigned loc;
};

#endif  // D20_TOKEN_INTERNAL_H
