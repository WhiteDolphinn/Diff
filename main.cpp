#include <stdio.h>
#include "tree_soft.h"
#include "diff.h"

int main()
{
    struct Node* n1 = nullptr;
    FILE* source_file = fopen("expession.txt", "r");

    if(source_file == nullptr)
    {
        printf("I can't open source_file\n");
        return 0;
    }

    read_expession_inorder(source_file, &n1);

    optimizate_tree(n1);
    tree_print(n1);
    printf("eval = %lf\n", eval(n1));

    struct Node* diff_n = diff(n1);
    optimizate_tree(diff_n);
    tree_print(diff_n);
    printf("eval_diff = %lf\n", eval(diff_n));
    delete_tree(n1);
    delete_tree(diff_n);
    fclose(source_file);

    return 0;
}
