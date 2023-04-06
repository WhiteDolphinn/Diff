#ifndef TREE_SOFT_H
#define TREE_SOFT_H

struct Node{
    struct Node* left;
    int type;
    double value;
    struct Node* right;
};

struct Node* create_node(int type, double value, struct Node* left = nullptr, struct Node* right = nullptr);
void push_node(struct Node* node, int type, double value);
//Node* delete_node(struct Node* node);
void delete_tree(struct Node* node);
void delete_tree_without_root(struct Node* node);
void tree_print(struct Node* tree);
void tree_print_inorder(struct Node* tree);
struct Node* copy_node(struct Node* node);
void optimizate_tree(struct Node* node);
bool is_number_tree(struct Node* node);
void merge_nodes(struct Node* in_node, struct Node* out_node);

#endif
