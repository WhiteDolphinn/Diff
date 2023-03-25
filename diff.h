#ifndef DIFF_H
#define DIFF_H

#include "tree_soft.h"

enum Type{
    NUMBER = 0,
    ADD = 1,
    SUB = 2,
    MUL = 3,
    DIV = 4,
};

double eval(struct Node* node);

#endif
