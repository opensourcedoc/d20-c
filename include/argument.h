#ifndef D20_ARGUMENT_H
#define D20_ARGUMENT_H

#include "parsing_event.h"

typedef struct parsing_result ParsingResult;

ParsingResult * parsing_result_new();
void parsing_result_free(void* self);
unsigned parsing_result_roll(ParsingResult *self);
unsigned parsing_result_dice(ParsingResult *self);
int parsing_result_modifier(ParsingResult *self);
PARSING_EVENT argument_pasre(ParsingResult *pr, char **args, int size);

#endif  // D20_ARGUMENT_H