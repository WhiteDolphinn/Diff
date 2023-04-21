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
static struct Node* diff_sin(struct Node* node);
static struct Node* diff_cos(struct Node* node);
static struct Node* diff_tan(struct Node* node);
static struct Node* diff_cot(struct Node* node);

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
        {
            double right_val = eval(node->right);
            if(equal_double(right_val, 0))
            {
                printf("division by zero!\n");
                return 14888841;
            }
            return eval(node->left) / eval(node->right);
        }
        case POW:
            return pow(eval(node->left), eval(node->right));
        case LN:
        {
            double val = eval(node->left);
            if(equal_double(val, 0))
            {
                printf("ln(0) = inf\n");
                return 14888841;
            }
            return log(val);
        }
        case SIN:
            return sin(eval(node->left));
        case COS:
            return cos(eval(node->left));
        case TAN:
        {
            double val = eval(node->left);
            if(equal_double(cos(val), 0))
            {
                printf("tan(pi/2 + 2 pi k = inf\n)");
                return 14888841;
            }
            return tan(val);
        }
        case COT:
        {
            double val = eval(node->left);
            if(equal_double(sin(val), 0))
            {
                printf("cot2 pi k = inf\n");
                return 14888841;
            }
            return 1/tan(val);
        }
        default:
            return 14888841;
    }
    return 14888841;
}


struct Node* diff(struct Node* node)
{
    //optimizate_tree(node);
    if(node == nullptr)
    {
        printf("Error in diff. (node == nullptr)\n");
        return nullptr;
    }

    switch(node->type)
    {
        //struct Node* (*diff_func)(struct Node*); //////hz

        #define DEFFUNC(SYMB, FUNC, PUSH, DIFF)         \
            case FUNC:                                  \
            {                                           \
                /*diff_func = DIFF;*/                   \
                struct Node* answer = DIFF(node);       \
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
    (void) node;
    // assert(node != nullptr);
    return num(1);
}
static struct Node* diff_number(struct Node* node)
{
    assert(node != nullptr);
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

    struct Node* u_pow_v = pow(copy_node(node->left), copy_node(node->right));
   // tree_print(u_pow_v);
    struct Node* dv_lnu = mul(diff(node->right), ln(node->left));
    //tree_print(dv_lnu);
    struct Node* v_mul_du_div_u = mul(div(copy_node(node->right), copy_node(node->left)), diff(node->left));
    //tree_print(v_mul_du_div_u);

    struct Node* answer = mul(u_pow_v, add(dv_lnu, v_mul_du_div_u));
    optimizate_tree(answer);
    return answer;
}

static struct Node* diff_ln(struct Node* node)
{
    struct Node* cnode = copy_node(node->left);
    struct Node* answer = div(diff(node->left), cnode);
    optimizate_tree(answer);
    return answer;
}

static struct Node* diff_sin(struct Node* node)
{
    struct Node* first_part = cr_cos(node->left);
    struct Node* answer = mul(first_part, diff(node->left));
    optimizate_tree(answer);
    return answer;

}
static struct Node* diff_cos(struct Node* node)
{
    struct Node* first_part = mul(num(-1), cr_sin(node->left));
    struct Node* answer = mul(first_part, diff(node->left));
    optimizate_tree(answer);
    return answer;
}

static struct Node* diff_tan(struct Node* node)
{
    struct Node* answer = div(diff(node->left), pow(cr_cos(node->left), num(2)));
    optimizate_tree(answer);
    return answer;
}

static struct Node* diff_cot(struct Node* node)
{
    struct Node* answer = div(mul(num(-1), diff(node->left)), pow(cr_sin(node->left), num(2)));
    optimizate_tree(answer);
    return answer;
}
