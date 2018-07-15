#include <stdlib.h>

#include "utils.h"

char * space_get(size_t sz)
{
    char *ss;
            
    if (sz == 0) {
        ss = "";
    } else {
        ss = malloc(sz * sizeof(char));
                
        for (size_t i = 0; i < sz; i++) {
            ss[i] = ' ';
        }
                
        ss[sz] = '\0';
    }
    
    return ss;
}