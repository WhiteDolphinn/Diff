#ifndef TREE_SOFT_H
#define TREE_SOFT_H

struct Node{
    struct Node* left;
    int type;
    double value;
    struct Node* right;
};

Node* create_node(int type, double value, Node* left = nullptr, Node* right = nullptr);
void push_node(Node* node, int type, double value);
//Node* delete_node(struct Node* node);
void delete_tree(struct Node* node);
void tree_print(struct Node* tree);
void tree_print_inorder(struct Node* tree);

#endif
