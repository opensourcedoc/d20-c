#include <stdio.h>

#include "metadata.h"
#include "platform.h"
#include "print_info.h"


void print_help(FILE *stream)
{
    fprintf(stream, "Usage: %s [option]|[d20]%s", PROGRAM, SEP);
    fprintf(stream, "%s", SEP);
    fprintf(stream, "Parameters:%s", SEP);
    fprintf(stream, "\t-v, --version\tShow version number%s", SEP);
    fprintf(stream, "\t-h, --help\tShow help message%s", SEP);
    fprintf(stream, "\t-r, --roll\tSet *n* roll(s)%s", SEP);
    fprintf(stream, "\t-d, --dice\tSet *n*-faced dice%s", SEP);
    fprintf(stream, "\t-m, --modifier\tSet modifier to *n*%s", SEP);
    fprintf(stream, "%s", SEP);
    fprintf(stream, "Samples of d20:%s", SEP);
    fprintf(stream, "\t\"1d8\"\t\tOne 8-faced dice%s", SEP);
    fprintf(stream, "\t\"2d6\"\t\tTwo 6-faced dices%s", SEP);
    fprintf(stream, "\t\"1d10+1\"\tOne 10-faced dice with +1 modifier%s", SEP);
}