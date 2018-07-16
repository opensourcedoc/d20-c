#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lex.h"
#include "platform.h"
#include "token.h"
#include "token_internal.h"
#include "utils.h"


struct obj_lex {
    size_t curr;
    size_t index;
    size_t size;
    size_t capacity;
    char *input;
    Token **data;
};

typedef enum {
    STATE_INT,
    STATE_DICE,
    STATE_SIGN,
    STATE_ERROR,
    STATE_END
} STATE;


static STATE lex_num(ObjLex *obj);
static STATE lex_dice(ObjLex *obj);
static STATE lex_sign(ObjLex *obj);

static bool is_num(char c);
static bool is_dice(char c);
static bool is_sign(char c);

ObjLex * lex_new(char *input)
{
    ObjLex *obj = (ObjLex *) malloc(sizeof(ObjLex));
    if (!obj) {
        return obj;
    }
    
    obj->curr = 0;
    obj->index = 0;
    obj->size = 0;
    obj->capacity = 4;
    obj->input = input;
    obj->data = malloc(obj->capacity * sizeof(Token*));
    
    STATE s;
    size_t sz = strlen(input);
    // Lex the input string by a finite automata.
    while (obj->curr < sz) {
        if (is_num(input[obj->curr])) {
            s = lex_num(obj);
        } else if (is_dice(input[obj->curr])) {
            s = lex_dice(obj);
        } else if (is_sign(input[obj->curr])) {
            s = lex_sign(obj);
        } else {
            s = STATE_ERROR;
        }

        // Show an error message if something wrong.
        if (s == STATE_ERROR) {
            size_t ssz = obj->curr;
            char *ss = space_get(ssz);
            
            fprintf(stderr, "%s%s", obj->input, SEP);
            fprintf(stderr, "%s^ -- invalid character at %u: %c%s", 
                ss, (unsigned) ssz + 1, obj->input[ssz], SEP);
            
            if (ssz > 0) {
                free(ss);
            }

            // Exit the automata.
            goto FAIL;
        }
        // Stop the automata.
        else if (s == STATE_END) {
            break;
        }
    }
    
    return obj;

FAIL:
    lex_free(obj);
    obj = NULL;
    return obj;
}

static void lex_expend(ObjLex *obj);

static STATE lex_num(ObjLex *self)
{
    size_t sz = strlen(self->input);
    size_t i;
    for (i = self->curr; i < sz; i++) {
        if (is_num(self->input[i])) {
            continue;
        } else {
            break;
        }
    }

    size_t size = i - self->curr;
    char *s = malloc(size * sizeof(char));
    strncpy(s, self->input+self->curr, size);
    s[size] = '\0';
    
    Token *t = token_new(s, TOKEN_INT, self->curr);

    self->curr = i;  // Renew self->curr
    
    free(s);
    
    lex_expend(self);
    
    self->data[self->size] = t;
    self->size += 1;
    
    return STATE_INT;
}

static bool is_num(char c)
{
    return c == '1' || c == '2' || c == '3' || c == '4' || c == '5' ||
        c == '6' || c == '7' || c == '8' || c == '9' || c == '0';
}

static STATE lex_dice(ObjLex *self)
{
    char s[2] = {self->input[self->curr], '\0'};
    Token *t = token_new(s, TOKEN_DICE, self->curr);

    STATE st;
    if (self->curr+1 >= strlen(self->input)) {
        st = STATE_END;
    } else {
        st = STATE_DICE;
    }
    self->curr += 1;

    lex_expend(self);
    
    self->data[self->size] = t;
    self->size += 1;
    
    return st;
}

static bool is_dice(char c)
{
    return c == 'd' || c == 'D';
}

static STATE lex_sign(ObjLex *self)
{
    char s[2] = {self->input[self->curr], '\0'};
    Token *t = token_new(s, TOKEN_SIGN, self->curr);

    STATE st;
    if (self->curr+1 >= strlen(self->input)) {
        st = STATE_END;
    } else {
        st = STATE_SIGN;
    }
    self->curr += 1;

    lex_expend(self);

    self->data[self->size] = t;
    self->size += 1;
    
    return st;
}

static bool is_sign(char c)
{
    return c == '+' || c == '-';
}

static void lex_expend(ObjLex *self)
{
    if (self->size >= self->capacity) {
        self->capacity = self->size * 2;
        
        Token **dataOld = self->data;
        Token **dataNew = malloc(self->capacity * sizeof(Token*));
        
        for (size_t i = 0; i < self->size; i++) {
            dataNew[i] = dataOld[i];
        }
        
        free(dataOld);
        self->data = dataNew;
    }
}

void lex_free(void *self)
{
    if (!self) {
        return;
    }
    
    size_t sz = ((ObjLex *) self)->size;
    for (size_t i = 0; i < sz; i++) {
        token_free(((ObjLex *) self)->data[i]);
    }
    
    free(((ObjLex *) self)->data);
    
    free(self);
}

Token * lex_next(ObjLex *self)
{
    if (self->index >= self->size) {
        return NULL;
    }

    Token *t = self->data[self->index];
    self->index += 1;
    
    return t;
}
