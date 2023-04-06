#include <ctype.h>
#include <math.h>
#include "diff.h"
#include "text.h"
#include "tree_soft.h"
#include "dsl.h"

static void read_node_preorder(char* expr, Node** root);
static void read_node_inorder(char* expr, Node** root);
static void read_number_preorder(char* expr, Node** root, int* index);
static bool read_number_inorder(char* expr, Node** root, int* index);
static void read_sign_preorder(char* expr, Node** root, int* index);
static bool read_sign_inorder(char* expr, Node** root, int* index);
static void skip_spaces(char* expr , int* index);
static void read_variable_inorder(char* expr, Node** root, int* index);
static struct Node* diff_pow(struct Node* node);

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

void read_expession_preorder(FILE* source_file, Node** root)
{
    char expession[MAX_STR_LENGTH] = {};

    if(!fscanf(source_file, " %[^\n]", expession))
    {
        printf("I can't read expession\n");
        return;
    }

    read_node_preorder(expession, root);
}

static void read_node_preorder(char* expr, Node** root)
{
    static int index = 0;
    skip_spaces(expr ,&index);
    if(expr[index] != '(')
    {
        printf("Error in possition %d. Symbol is '%c' but expected '('\n", index, expr[index]);
        return;
    }

    *root = create_node(257, 257);

    index++;
    skip_spaces(expr, &index);

    switch(expr[index])
    {
        case '+': case '*': case '/': case '^':
        {
            read_sign_preorder(expr, root, &index);
            break;
        }

        case '-':
        {
            int old_index = index;
            index++;
            skip_spaces(expr, &index);
            switch(expr[index])
            {
                case '(':
                {
                    index = old_index;
                    read_sign_preorder(expr, root, &index);
                    break;
                }
                case '0'...'9':
                {
                    index = old_index;
                    read_number_preorder(expr, root, &index);
                    break;
                }
                default:
                {
                    printf("Error in position %d. Symbol is %c but expected number or '('.\n", index, expr[index]);
                    return;
                }
            }

            skip_spaces(expr, &index);
            break;
        }

        case '0'...'9':
        {
            read_number_preorder(expr, root, &index);
            break;
        }
        default:
        {
            printf("Error in position %d. Symbol is %c but expected number or '+' '-' '*' '/'.\n", index, expr[index]);
            return;
        }
    }

    if(expr[index] != ')')
    {
        printf("Error in possition %d. Symbol is '%c' but expected ')'\n", index, expr[index]);
        return;
    }

    index++;
    skip_spaces(expr, &index);
}

static void read_number_preorder(char* expr, Node** root, int* index)
{
    double num = 0;
    sscanf(expr + *index, " %lf", &num);
    push_node(*root, NUMBER, num);

    while(isdigit(expr[*index]) || expr[*index] == '.' || expr[*index] == '-')
        (*index)++;

    skip_spaces(expr, index);
}

static void read_sign_preorder(char* expr, Node** root, int* index)
{
    push_node(*root, expr[*index], expr[*index]);
    (*index)++;
    skip_spaces(expr, index);
    read_node_preorder(expr, &((*root)->left));
    skip_spaces(expr, index);
    read_node_preorder(expr, &((*root)->right));
}

static void skip_spaces(char* expr, int* index)
{
    while(isspace(expr[*index]))
        (*index)++;

    return;
}

void read_expession_inorder(FILE* source_file, Node** root)
{
    char expession[MAX_STR_LENGTH] = {};


    if(!fscanf(source_file, " %[^\n]", expession))
    {
        printf("I can't read expession\n");
        return;
    }

    read_node_inorder(expession, root);
}

static void read_node_inorder(char* expr, Node** root)
{
    static bool is_correct_read = true;
    static int index = 0;

    if(is_correct_read == false)
        return;

    skip_spaces(expr, &index);
    if(expr[index] != '(')
    {
        printf("Error in possition %d. Symbol is '%c' but expected '('\n", index, expr[index]);
        return;
    }

    *root = create_node(257, 257);

    index++;
    skip_spaces(expr, &index);

    switch(expr[index])
    {
        case '(':
        {
            read_node_inorder(expr, &((*root)->left));
            is_correct_read = read_sign_inorder(expr, root, &index);
            read_node_inorder(expr, &((*root)->right));
            break;

        }
        case '0'...'9': case '-':
        {
            is_correct_read = read_number_inorder(expr, root, &index);
            break;
        }
        case 'A'...'Z': case 'a'...'z':
        {
            read_variable_inorder(expr, root, &index);
            break;
        }
        default:
        {
            printf("Error in position %d. Symbol is %c but expected number, variable or (.\n", index, expr[index]);
            return;
        }
    }

    if(expr[index] != ')')
    {
        printf("Error in possition %d. Symbol is '%c' but expected ')'\n", index, expr[index]);
        return;
    }

    index++;
    skip_spaces(expr, &index);
}

static bool read_number_inorder(char* expr, Node** root, int* index)
{
    double num = 0;

    if(sscanf(expr + *index, " %lf", &num) != 1)
    {
        printf("Error in possition %d. Symbol is '%c' but expected number\n", *index, expr[*index]);
        return false;
    }

    push_node(*root, NUMBER, num);

    while(isdigit(expr[*index]) || expr[*index] == '.' || expr[*index] == '-')
        (*index)++;

    skip_spaces(expr, index);
    return true;
}

static bool read_sign_inorder(char* expr, Node** root, int* index)
{
    skip_spaces(expr, index);

    if(expr[*index] != '+' && expr[*index] != '-' && expr[*index] != '*' && expr[*index] != '/' && expr[*index] != '^')
    {
        printf("Error in possition %d. Symbol is '%c' but expected '=', '-', '*', '/'\n", *index, expr[*index]);
        return false;
    }

    push_node(*root, expr[*index], expr[*index]);
    (*index)++;

    skip_spaces(expr, index);
    return true;
}

static void read_variable_inorder(char* expr, Node** root, int* index)
{
    skip_spaces(expr, index);

    if(expr[(*index) + 1] == ')' || isspace(expr[(*index) + 1]))
    {
        push_node(*root, VAR, expr[*index]);
        (*index)++;
        skip_spaces(expr, index);

        return;
    }

    char func[MAX_STR_LENGTH] = {};
    sscanf(expr + (*index), "%[^(]", func);
    printf("%s\n", func);

    if(!stricmp(func, "ln"))/////////////сделать кодогенерацию
    {
        push_node(*root, LN, LN);

        for(size_t i = 0; i < strlen(func); i++)
            (*index)++;

        read_node_inorder(expr, &((*root)->left));
        (*root)->right = nullptr;

        skip_spaces(expr, index);
        return;
    }

    skip_spaces(expr, index);
}

struct Node* diff(struct Node* node)
{
    //optimizate_tree(node);
    switch(node->type)
    {
        case VAR:       return num(1);
        case NUMBER:    return num(0);
        case ADD:
        {
            struct Node* answer = add(diff(node->left), diff(node->right));
            optimizate_tree(answer);
            return answer;
        }
        case SUB:
        {
            struct Node* answer = sub(diff(node->left), diff(node->right));
            optimizate_tree(answer);
            return answer;
        }
        case MUL:
        {
            struct Node* cl = copy_node(node->left);
            struct Node* cr = copy_node(node->right);
            struct Node* dl = diff(node->left);
            struct Node* dr = diff(node->right);

            struct Node* answer = add(mul(cl, dr), mul(dl, cr));
            optimizate_tree(answer);
            //tree_print(branch);
            return answer;
        }

        case DIV:
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

        case POW:
        {
            struct Node* answer = diff_pow(node);
            optimizate_tree(answer);
            return answer;
        }

        default:
        {
            printf("Uncknown type: %d  (%c)\n", node->type, (char)node->type);
            return nullptr;
        }
    }
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

    /*struct Node* cl = copy_node(node->left);
    struct Node* cr = copy_node(node->right);
    struct Node* dl = diff(node->left);
    struct Node* dr = diff(node->right);

    struct Node* u_pow_v = pow(cl, cr);
    struct Node* diffv_lnu = mul(dr, );*/
    return nullptr;
}
