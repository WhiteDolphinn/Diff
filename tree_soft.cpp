#include "tree_soft.h"
#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include "dotter.h"

static void node_print(struct Node* node);

Node* create_node(int type, double value, Node* left, Node* right)
{
    struct Node* node = (struct Node*)calloc(1, sizeof(struct Node));

    node->type = type;
    node->value = value;
    node->left = left;
    node->right = right;

    return node;
}

void delete_tree(struct Node* node)
{
    if(node == nullptr)
        return;

    delete_tree(node->left);
    delete_tree(node->right);

    free(node);
}

void tree_print(struct Node* tree)
{
    if(tree == nullptr)
        return;

    graph_start();
    graph_add_root(tree);

    node_print(tree);

    graph_end();
}

static void node_print(struct Node* node)
{
    if(node == nullptr)
        return;

    node_print(node->left);
    node_print(node->right);

    if(node->type == 0)
        graph_add_dot(node, node->value, node->type, node->left, node->right, "#D0FFD0");
    else
        graph_add_dot(node, node->value, node->type, node->left, node->right, "#FFD0D0");

    if(node->left != nullptr)
        graph_add_arrow(node, node->left, "#0000FF");

    if(node->right != nullptr)
        graph_add_arrow(node, node->right, "#FF0000");
}


