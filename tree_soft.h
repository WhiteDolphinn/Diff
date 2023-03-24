#ifndef TREE_SOFT_H
#define TREE_SOFT_H

#define POISON 14888841
typedef int data_t;

struct Node{
    struct Node* left;
    data_t value;
    struct Node* right;
};

struct Tree{
    struct Node* root;
};

void tree_ctor(struct Tree* tree);
void tree_dtor(struct Node* tree);
void tree_push(struct Node* tree, data_t value, char side);
void tree_pop(struct Node* tree);

#endif
