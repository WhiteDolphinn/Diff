#ifndef READ_TREE_H
#define READ_TREE_H

#include <stdio.h>
#include "tree_soft.h"

void read_expession_preorder(FILE* source_file, Node** root);
void read_expession_inorder(FILE* source_file, Node** root);
void skip_spaces(char* expr , int* index);

#endif
