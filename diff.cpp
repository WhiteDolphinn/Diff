#include <ctype.h>
#include "diff.h"
#include "text.h"
#include "tree_soft.h"

static void read_node_preorder(char* expr, Node** root);
static void read_node_inorder(char* expr, Node** root);
static void read_number_preorder(char* expr, Node** root, int* index);
static void read_number_inorder(char* expr, Node** root, int* index);
static void read_sign_preorder(char* expr, Node** root, int* index);
static void read_sign_inorder(char* expr, Node** root, int* index);
static void skip_spaces(char* expr , int* index);



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
        case '+': case '*': case '/':
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
    static int index = 0;
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
            read_sign_inorder(expr, root, &index);
            read_node_inorder(expr, &((*root)->right));
            break;

        }
        case '0'...'9': case '-':
        {
            read_number_inorder(expr, root, &index);
            break;
        }
        default:
        {
            printf("Error in position %d. Symbol is %c but expected number or (.\n", index, expr[index]);
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

static void read_number_inorder(char* expr, Node** root, int* index)
{
    double num = 0;
    sscanf(expr + *index, " %lf", &num);
    push_node(*root, NUMBER, num);

    while(isdigit(expr[*index]) || expr[*index] == '.' || expr[*index] == '-')
        (*index)++;

    skip_spaces(expr, index);
}

static void read_sign_inorder(char* expr, Node** root, int* index)
{
    skip_spaces(expr, index);
    push_node(*root, expr[*index], expr[*index]);
    (*index)++;
    skip_spaces(expr, index);
}
