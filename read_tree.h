#ifndef READ_TREE_H
#define READ_TREE_H

#include <stdio.h>
#include <assert.h>
#include "tree_soft.h"

void read_expession_preorder(FILE* source_file, Node** root);
void read_expession_inorder(FILE* source_file, Node** root);
void skip_spaces(char* expr , int* index);

void func_to_str(int type, char* func);
bool is_func(int type);

#endif
