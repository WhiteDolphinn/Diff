#include "tree_soft.h"
#include "diff.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "log.h"
#include "dotter.h"

static void node_print(struct Node* node);
static int equal_double(double a, double b);

struct Node* create_node(int type, double value, struct Node* left, struct Node* right)
{
    struct Node* node = (struct Node*)calloc(1, sizeof(struct Node));

    node->type = type;
    node->value = value;
    node->left = left;
    node->right = right;

    return node;
}

void push_node(struct Node* node, int type, double value)
{
    node->type = type;
    node->value = value;
}

void delete_tree(struct Node* node)
{
    if(node == nullptr)
        return;

    delete_tree(node->left);
    delete_tree(node->right);

    free(node);
}

void delete_tree_without_root(struct Node* node)
{
    delete_tree(node->left);
    delete_tree(node->right);
    node->left = nullptr;
    node->right = nullptr;
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

    if(node->type == NUMBER)
        graph_add_dot(node, node->value, node->type, node->left, node->right, "#FFD0D0");
    else if(node->type == ADD || node->type == SUB || node->type == MUL || node->type == DIV)
        graph_add_dot(node, node->value, node->type, node->left, node->right, "#D0FFD0");
    else if(node->type == VAR)
        graph_add_dot(node, node->value, node->type, node->left, node->right, "#D0D0FF");
    else
        graph_add_dot(node, node->value, node->type, node->left, node->right, "#FF0000");

    if(node->left != nullptr)
        graph_add_arrow(node, node->left, "#0000FF");

    if(node->right != nullptr)
        graph_add_arrow(node, node->right, "#FF0000");
}

void tree_print_inorder(struct Node* tree)
{
    if(tree == nullptr)
        return;

    printf("( ");

    tree_print_inorder(tree->left);

    if(tree->type == 0)
        printf("%lf ", tree->value);
    else
    {
        switch((int)tree->value)
        {
            case 1:   printf("+ "); break;
            case 2:   printf("- "); break;
            case 3:   printf("* "); break;
            case 4:   printf("/ "); break;
            default:  printf("bebra\n"); break;
        }
    }

    tree_print_inorder(tree->right);

    printf(") ");
}

struct Node* copy_node(struct Node* node)
{
    if(node == nullptr)
        return nullptr;

    return create_node(node->type, node->value, copy_node(node->left), copy_node(node->right));
}

void optimizate_tree(struct Node* node)
{
    if(node->left == nullptr || node->right == nullptr)
        return;

    //printf("(*node)->left = %p\n", (*node)->left);
    //printf("(*node)->right = %p\n", (*node)->right);

    if(equal_double(node->left->value, 0) && equal_double(node->right->value, 0))
    {
        delete_tree_without_root(node);
        push_node(node, NUMBER, 0);
        return;
    }
    if(node->type == MUL && (equal_double(node->left->value, 0) || equal_double(node->right->value, 0)))
    {
        delete_tree_without_root(node);
        push_node(node, NUMBER, 0);
        return;
    }
    if(is_number_tree(node))
    {
        double num = eval(node);
        delete_tree_without_root(node);
        push_node(node, NUMBER, num);
    }
    //printf("Я ёбанная параша(функция оптимизации) передаю %p и %p\n", &((*node)->left), &((*node)->right));

    if(node->left == nullptr || node->right == nullptr)
        return;

    optimizate_tree(node->left);
    optimizate_tree(node->right);
}

static int equal_double(double a, double b)
{
    return fabs(a - b) < 1e-7;
}

bool is_number_tree(struct Node* node)
{

    /*switch(node->type)
    {
        case NUMBER:    break;
        case VAR:   is_number = false; break;

        case ADD: case SUB: case MUL: case DIV:
        {
            is_number = is_number_tree(node->left);
            if(is_number == true)
                is_number = is_number_tree(node->right);
            break;
        }
        default:
            return false;
    }
    return is_number;*/

    switch(node->type)
    {
        case NUMBER: return true;

        case VAR: return false;

        case ADD: case SUB: case MUL: case DIV:
            return is_number_tree(node->left) && is_number_tree(node->right);

        default: return false;

    }
    return false;
}

