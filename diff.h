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
    POW = '^',
    VAR = 'x',
    LN = 'l'
};

double eval(struct Node* node);
void read_expession_preorder(FILE* source_file, Node** root);
void read_expession_inorder(FILE* source_file, Node** root);
struct Node* diff(struct Node* node);

#endif
