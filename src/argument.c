#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "argument.h"
#include "platform.h"

struct parsing_result {
    unsigned int roll;
    unsigned int dice;
    int modifier;
    char *str;
};

ParsingResult * parsing_result_new()
{
    ParsingResult *pr = (ParsingResult *) malloc(sizeof(ParsingResult));
    if (!pr) {
        return pr;
    }
    
    // Default to 1d6
    pr->roll = 1;
    pr->dice = 6;
    pr->modifier = 0;
    
    return pr;
}

void parsing_result_free(void* self)
{
    if (!self) {
        return;
    }
    
    free(self);
}

unsigned parsing_result_roll(ParsingResult *self)
{
    assert(self);
    
    return self->roll;
}

void parsing_result_set_roll(ParsingResult *self, unsigned r)
{
    assert(self);
    
    self->roll = r;
}

unsigned parsing_result_dice(ParsingResult *self)
{
    assert(self);
    
    return self->dice;
}

void parsing_result_set_dice(ParsingResult *self, unsigned d)
{
    assert(self);
    
    self->dice = d;
}

int parsing_result_modifier(ParsingResult *self)
{
    assert(self);
    
    return self->modifier;
}

void parsing_result_set_modifier(ParsingResult *self, int m)
{
    assert(self);
    
    self->modifier = m;
}

char * parsing_result_string(ParsingResult *self)
{
    assert(self);
    
    return self->str;
}

PARSING_EVENT argument_pasre(ParsingResult *pr, char **args, int size)
{

    for (int i = 1; i < size; i++) {
        // Show version number.
        if (strcmp(args[i], "-v") == 0 || strcmp(args[i], "--version") == 0) {
            return PARSING_EVENT_VERSION;
        }
        // Show help message.
        else if (strcmp(args[i], "-h") == 0 || strcmp(args[i], "--help") == 0) {
            return PARSING_EVENT_HELP;
        }
        // Roll
        else if (strcmp(args[i], "-r") == 0 || strcmp(args[i], "--roll") == 0) {
            if (i+1 >= size) {
                fprintf(stderr, "No valid roll%s", SEP);
                return PARSING_EVENT_ERROR;
            }

            unsigned int r = strtoul(args[i+1], NULL, 10);
            if (args[i+1][0] == '-' || r == 0) {
                fprintf(stderr, "Invalid roll: %s%s", args[i+1], SEP);
                errno = 0;
                return PARSING_EVENT_ERROR;
            }
            
            
            parsing_result_set_roll(pr, r);

            i++;  // Go one step further.
        }
        // Dice
        else if (strcmp(args[i], "-d") == 0 || strcmp(args[i], "--dice") == 0) {
            if (i+1 >= size) {
                fprintf(stderr, "No valid dice%s", SEP);
                return PARSING_EVENT_ERROR;
            }
            
            unsigned int d = strtoul(args[i+1], NULL, 10);
            if (args[i+1][0] == '-' || d == 0) {
                fprintf(stderr, "Invalid dice: %s%s", args[i+1], SEP);
                errno = 0;
                return PARSING_EVENT_ERROR;
            }

            parsing_result_set_dice(pr, d);
            
            i++;  // Go one step further.
        }
        // Modifier
        else if (strcmp(args[i], "-m") == 0 || strcmp(args[i], "--modifier") == 0) {
            if (i+1 >= size) {
                fprintf(stderr, "No valid modifier%s", SEP);
                return PARSING_EVENT_ERROR;
            }
            
            
            int m = strtol(args[i+1], NULL, 10);
            if (errno == ERANGE) {
                fprintf(stderr, "Invalid modifier: %s%s", args[i+1], SEP);
                errno = 0;
                return PARSING_EVENT_ERROR;
            }
            
            parsing_result_set_modifier(pr, m);
            
            i++;  // Go one step further.
        } else {
            pr->str = args[i];
            break;
        }
    }
    
    return PARSING_EVENT_SUCCESS;
}
