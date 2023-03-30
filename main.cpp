#include <stdio.h>
#include "tree_soft.h"
#include "diff.h"

int main()
{
    /*struct Node* n1 = create_node(NUMBER, 6.3);
    struct Node* n2 = create_node(NUMBER, 3.7);
    struct Node* n3 = create_node(ADD, 1, n1, n2);
    struct Node* n4 = create_node(NUMBER, 5);
    struct Node* n5 = create_node(DIV, 4, n4, n3);
    tree_print(n5);
    tree_print_inorder(n5);
    printf("eval(n5) = %lf\n", eval(n5));
    delete_tree(n5);*/
    struct Node* n1 = nullptr;
    FILE* source_file = fopen("expession.txt", "r");

    if(source_file == nullptr)
    {
        printf("I can't open source_file\n");
        return 0;
    }

    read_expession(source_file, &n1);
    tree_print(n1);
    printf("eval = %lf\n", eval(n1));

    delete_tree(n1);
    fclose(source_file);

    return 0;
}
