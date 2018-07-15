#ifndef D20_ARGUMENT_H
#define D20_ARGUMENT_H

#include "parsing_event.h"

typedef struct parsing_result ParsingResult;

ParsingResult * parsing_result_new();
void parsing_result_free(void* self);
unsigned parsing_result_roll(ParsingResult *self);
void parsing_result_set_roll(ParsingResult *self, unsigned r);
unsigned parsing_result_dice(ParsingResult *self);
void parsing_result_set_dice(ParsingResult *self, unsigned d);
int parsing_result_modifier(ParsingResult *self);
void parsing_result_set_modifier(ParsingResult *self, int m);
char * parsing_result_string(ParsingResult *self);
PARSING_EVENT argument_pasre(ParsingResult *pr, char **args, int size);

#endif  // D20_ARGUMENT_H