#ifndef D20_LEX_H
#define D20_LEX_H

#include "token.h"

typedef struct obj_lex ObjLex;

ObjLex * lex_new(char *input);
void lex_free(void *self);
Token * lex_next(ObjLex *self);

#endif  // D20_LEX_H