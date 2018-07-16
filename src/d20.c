#include <stdio.h>
#include <stdlib.h>

#include "argument.h"
#include "dice.h"
#include "eval.h"
#include "lex.h"
#include "metadata.h"
#include "parsing_event.h"
#include "platform.h"
#include "print_info.h"
#include "token.h"


int main(int argc, char *argv[])
{
    ParsingResult *pr = parsing_result_new();
    if (!pr) {
        return EXIT_FAILURE;
    }
    
    PARSING_EVENT pv = argument_pasre(pr, argv, argc);
    
    if (pv == PARSING_EVENT_VERSION) {
        printf("%s%s", VERSION, SEP);
        goto FREE;
    }
    else if (pv == PARSING_EVENT_HELP) {
        print_help(stdout);
        goto FREE;
    }
    else if (pv == PARSING_EVENT_ERROR) {
        parsing_result_free(pr);
        return EXIT_FAILURE;
    }
    
    char *input = parsing_result_string(pr);
    if (!input) {
        goto ROLL;
    }

    ParsingResult *out = parsing_result_new();
    if (!eval(input, out)) {
        parsing_result_free(out);
        parsing_result_free(pr);
        return EXIT_FAILURE;
    }
    
    parsing_result_set_roll(pr, parsing_result_roll(out));
    parsing_result_set_dice(pr, parsing_result_dice(out));
    parsing_result_set_modifier(pr, parsing_result_modifier(out));

    parsing_result_free(out);
    
    int result;
ROLL:
    result = dice_roll(
        parsing_result_roll(pr),
        parsing_result_dice(pr),
        parsing_result_modifier(pr)
    );

    printf("%d%s", result, SEP);

FREE:
    parsing_result_free(pr);
    
    return 0;
}
