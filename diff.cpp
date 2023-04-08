#include <ctype.h>
#include <math.h>
#include "diff.h"
#include "text.h"
#include "tree_soft.h"
#include "read_tree.h"
#include "dsl.h"

static struct Node* diff_var(struct Node* node);
static struct Node* diff_number(struct Node* node);
static struct Node* diff_add(struct Node* node);
static struct Node* diff_sub(struct Node* node);
static struct Node* diff_mul(struct Node* node);
static struct Node* diff_div(struct Node* node);
static struct Node* diff_pow(struct Node* node);
static struct Node* diff_ln(struct Node* node);

double eval(struct Node* node)
{
    if(!is_number_tree(node))
        return 14888841;

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
        case POW:
            return pow(eval(node->left), eval(node->right));
        case LN:
            return log(eval(node->left));
        default:
            return 14888841;
    }
    return 14888841;
}


struct Node* diff(struct Node* node)
{
    //optimizate_tree(node);
    switch(node->type)
    {
        struct Node* (*diff_func)(struct Node*); //////hz

        #define DEFFUNC(SYMB, FUNC, PUSH, DIFF)         \
            case FUNC:                                  \
            {                                           \
                diff_func = DIFF;                       \
                struct Node* answer = diff_func(node);  \
                optimizate_tree(answer);                \
                return answer;                          \
            }                                           \

        #include "diff_funcs.h"

        #undef DEFFUNC

        default:
        {
            printf("Uncknown type: %d  (%c)\n", node->type, (char)node->type);
            return nullptr;
        }
    }
}

static struct Node* diff_var(struct Node* node)
{
    return num(1);
}
static struct Node* diff_number(struct Node* node)
{
    return num(0);
}
static struct Node* diff_add(struct Node* node)
{
    struct Node* answer = add(diff(node->left), diff(node->right));
    optimizate_tree(answer);
    return answer;
}
static struct Node* diff_sub(struct Node* node)
{
    struct Node* answer = sub(diff(node->left), diff(node->right));
    optimizate_tree(answer);
    return answer;
}
static struct Node* diff_mul(struct Node* node)
{
    struct Node* cl = copy_node(node->left);
    struct Node* cr = copy_node(node->right);
    struct Node* dl = diff(node->left);
    struct Node* dr = diff(node->right);

    struct Node* answer = add(mul(cl, dr), mul(dl, cr));
    optimizate_tree(answer);
    return answer;
}

static struct Node* diff_div(struct Node* node)
{
    struct Node* cl = copy_node(node->left);
    struct Node* cr = copy_node(node->right);
    struct Node* dl = diff(node->left);
    struct Node* dr = diff(node->right);
    struct Node* r_2 = mul(copy_node(node->right), copy_node(node->right));

    struct Node* first_part = sub(mul(dl, cr), mul(cl, dr));
    struct Node* answer = div(first_part, r_2);
    optimizate_tree(answer);
    return answer;
}

static struct Node* diff_pow(struct Node* node)
{
    if(is_number_tree(node->left) && is_number_tree(node->right))
        return num(0);

    if(is_number_tree(node->right))
    {
        double node_pow = eval(node->right);

        struct Node* first_part = num(node_pow);
        struct Node* second_part = pow(copy_node(node->left), num(node_pow - 1));
        struct Node* third_part = diff(node->left);

        struct Node* fs_part = mul(first_part, second_part);

        struct Node* answer = mul(fs_part, third_part);
        //tree_print(buf);
        return answer;
    }

    if(is_number_tree(node->left))
    {
        struct Node* first_part = num(log(node->left->value));
        struct Node* second_part = copy_node(node);
        struct Node* third_part = diff(node->right);

        struct Node* answer = mul(mul(first_part, second_part), third_part);
        return answer;
    }

    return nullptr;
}

static struct Node* diff_ln(struct Node* node)
{
    struct Node* cnode = copy_node(node->left);
    struct Node* answer = div(diff(node->left), cnode);
    optimizate_tree(answer);
    return answer;
}
