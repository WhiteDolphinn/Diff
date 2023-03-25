#include <stdio.h>
#include "tree_soft.h"

int main()
{
    struct Node* n1 = create_node(1, 6.3);
    struct Node* n2 = create_node(1, 3.6);
    struct Node* n3 = create_node(0, 1, n1, n2);
    tree_print(n3);
    delete_tree(n3);

    return 0;
}
