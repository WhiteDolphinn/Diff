#ifndef DSL_H
#define DSL_H

#include "diff.h"

struct Node* add(struct Node* node1, struct Node* node2);
struct Node* sub(struct Node* node1, struct Node* node2);
struct Node* mul(struct Node* node1, struct Node* node2);
struct Node* div(struct Node* node1, struct Node* node2);

struct Node* add(struct Node* node1, struct Node* node2)
{
    return create_node(ADD, ADD, node1, node2);
}

struct Node* sub(struct Node* node1, struct Node* node2)
{
    return create_node(SUB, SUB, node1, node2);
}

struct Node* mul(struct Node* node1, struct Node* node2)
{
    return create_node(MUL, MUL, node1, node2);
}

struct Node* div(struct Node* node1, struct Node* node2)
{
    return create_node(DIV, DIV, node1, node2);
}

#endif
