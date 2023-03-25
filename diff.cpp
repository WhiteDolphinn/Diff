#include "diff.h"

double eval(struct Node* node)
{
    if(node->type == NUMBER)
        return node->value;

    switch(node->type)
    {
        case ADD:
            return eval(node->left) + eval(node->right);
        case SUB:
            return eval(node->left) - eval(node->right);
        case MUL:
            return eval(node->left) * eval(node->right);
        case DIV:
            return eval(node->left) / eval(node->right);
    }
    return 14888841;
}
