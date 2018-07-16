#include <stdlib.h>
#include <time.h>

#include "dice.h"


int dice_roll(unsigned roll, unsigned dice, int modifier)
{
    srand((unsigned) time(NULL));

    int result = 0;

    for (unsigned i = 0; i < roll; i++) {
        result += rand() % dice + 1;
    }
    
    result += modifier;
    
    return result;
}