#ifndef READ_TREE_H
#define READ_TREE_H

#include <stdio.h>
#include <assert.h>
#include "tree_soft.h"

void read_expession_preorder(FILE* source_file, Node** root);
void read_expession_inorder(FILE* source_file, Node** root);
int read_expession_rec_descent(FILE* source_file, Node** root);
void skip_spaces(char* expr , int* index);

void func_to_str(int type, char* func);
bool is_func(int type);

enum Errors{
    SYNTAX_ERROR_IN_GET_G = 1,
    ERROR_ROOT_POINTER = 2,
    ERROR_READ_EXPESSION = 3,
    ERROR_BUFFER_OVERFLOW = 4,
    SYNTAX_ERROR_IN_GET_N = 5,
    SYNTAX_ERROR_IN_GET_E = 6,
    SYNTAX_ERROR_IN_GET_T = 7,
    SYNTAX_ERROR_IN_GET_F = 8,
    SYNTAX_ERROR = 257,
};

#endif
