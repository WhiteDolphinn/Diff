#ifndef DIFF_H
#define DIFF_H

#include "tree_soft.h"
#include <stdio.h>

enum Type{
    NUMBER = 0,
    ADD = '+',
    SUB = '-',
    MUL = '*',
    DIV = '/',
};

double eval(struct Node* node);
void read_expession(FILE* source_file, Node** root);

#endif
