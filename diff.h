#ifndef DIFF_H
#define DIFF_H

#include "tree_soft.h"
#include <stdio.h>

#define DEFFUNC(SYMB, FUNC, PUSH, DIFF)\
    FUNC = SYMB,

enum Type{
    /*NUMBER = 0,
    ADD = '+',
    SUB = '-',
    MUL = '*',
    DIV = '/',
    POW = '^',
    VAR = 'x',
    LN = 'l'*/
    #include "diff_funcs.h"
    #undef DEFFUNC
};



double eval(struct Node* node);
struct Node* diff(struct Node* node);

#endif
