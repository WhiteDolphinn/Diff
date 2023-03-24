#include <stdio.h>
#include "tree_soft.h"

int main()
{
    Tree tree = {};
    tree_ctor(&tree);
    tree_push(tree.root, 5, 'r');
    tree_push(tree.root, 10, 'l');
    tree_dtor(tree.root);
    return 0;
}
