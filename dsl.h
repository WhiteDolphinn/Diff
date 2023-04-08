#ifndef DSL_H
#define DSL_H

#include "diff.h"
#include "tree_soft.h"

struct Node* add(struct Node* node1, struct Node* node2);
struct Node* sub(struct Node* node1, struct Node* node2);
struct Node* mul(struct Node* node1, struct Node* node2);
struct Node* div(struct Node* node1, struct Node* node2);
struct Node* pow(struct Node* node1, struct Node* node2);
struct Node* num(double number);
struct Node* ln(struct Node* node);
struct Node* cr_sin(struct Node* node);
struct Node* cr_cos(struct Node* node);

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

struct Node* pow(struct Node* node1, struct Node* node2)
{
    return create_node(POW, POW, node1, node2);
}

struct Node* num(double number)
{
    return create_node(NUMBER, number);
}

struct Node* ln(struct Node* node)
{
    return create_node(LN, LN, node);
}

struct Node* cr_sin(struct Node* node)
{
    return create_node(SIN, SIN, copy_node(node));
}

struct Node* cr_cos(struct Node* node)
{
    return create_node(COS, COS, copy_node(node));
}

#endif
