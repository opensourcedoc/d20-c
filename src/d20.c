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
    // Create a ParsingResult object.
    ParsingResult *pr = parsing_result_new();
    if (!pr) {
        return EXIT_FAILURE;
    }
    
    // Parse command-line arguments.
    PARSING_EVENT pv = argument_pasre(pr, argv, argc);
    
    // Branch the program as needed.
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
    
    // Check if any d20 string from those arguments.
    char *input = parsing_result_string(pr);
    if (!input) {
        goto ROLL;
    }

    // Evaluate d20 string as needed.
    ParsingResult *out = parsing_result_new();
    if (!eval(input, out)) {
        parsing_result_free(out);
        parsing_result_free(pr);
        return EXIT_FAILURE;
    }
    
    // Update the ParsingResult object.
    parsing_result_set_roll(pr, parsing_result_roll(out));
    parsing_result_set_dice(pr, parsing_result_dice(out));
    parsing_result_set_modifier(pr, parsing_result_modifier(out));

    parsing_result_free(out);
    
    // Roll the dice.
    int result;
ROLL:
    result = dice_roll(
        parsing_result_roll(pr),
        parsing_result_dice(pr),
        parsing_result_modifier(pr)
    );

    printf("%d%s", result, SEP);

    // Free system resources.
FREE:
    parsing_result_free(pr);
    
    return 0;
}
