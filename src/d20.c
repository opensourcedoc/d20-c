#include <stdio.h>
#include <stdlib.h>
#include "argument.h"
#include "parsing_event.h"
#include "metadata.h"
#include "platform.h"
#include "print_info.h"
#include "dice.h"

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
    
    int result = dice_roll(
        parsing_result_roll(pr),
        parsing_result_dice(pr),
        parsing_result_modifier(pr)
    );

    printf("%d%s", result, SEP);

FREE:
    parsing_result_free(pr);
    
    return 0;
}
