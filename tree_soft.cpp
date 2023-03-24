#include "tree_soft.h"
#include <stdio.h>
#include <stdlib.h>

void tree_ctor(struct Tree* tree)
{
    tree->root = (struct Node*)calloc(1, sizeof(struct Node));
    if(tree == nullptr)
    {
        printf("Error in ctor of tree\n");
        return;
    }

    tree->root->value = POISON;
    tree->root->left = nullptr;
    tree->root->right = nullptr;
}

void tree_dtor(struct Node* tree)
{
    if(tree->left != nullptr)
        tree_dtor(tree->left);

    if(tree->right != nullptr)
        tree_dtor(tree->right);

    free(tree);

}

void tree_push(struct Node* tree, data_t value, char side)
{
    struct Node* new_node = (struct Node*)calloc(1, sizeof(struct Node));
    new_node->value = value;
    new_node->left = nullptr;
    new_node->right = nullptr;

    if(side == 'r')
        tree->right = new_node;
    else if(side == 'l')
            tree->left = new_node;
        else
            printf("Error with side. %c is uncorrent side. (r - right, l - left)\n", side);

}

void tree_pop(struct Node* tree)
{

}
