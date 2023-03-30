#include <ctype.h>
#include "diff.h"
#include "text.h"
#include "tree_soft.h"

static void read_node(char* expr, Node** root);
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

void read_expession(FILE* source_file, Node** root)
{
    char expession[MAX_STR_LENGTH] = {};
    //int current_index = 0;

    if(!fscanf(source_file, " %[^\n]", expession))
    {
        printf("I can't read expession\n");
        return;
    }
    /*for(int i = 0; expression[i] != '\0'; i++)
        read_node(expression + i, i, root);*/
    read_node(expession, root);
}

static void read_node(char* expr, Node** root)
{
    static int index = 0;
    skip_spaces(expr ,&index);
    if(expr[index] != '(')
    {
        printf("Error in possition %d. Symbol is '%c' but expected '('\n", index, expr[index]);
        return;
    }

    *root = create_node(228, 228);

    index++;
    skip_spaces(expr, &index);

    switch(expr[index])
    {
        case '+':
        {
            push_node(*root, ADD, ADD);
            index++;
            skip_spaces(expr, &index);
            read_node(expr, &((*root)->left));
            skip_spaces(expr, &index);
            read_node(expr, &((*root)->right));
            break;
        }

        case '0'...'9':
        {
            int num = 0;
            sscanf(expr + index, " %d", &num);
            push_node(*root, NUMBER, num);

            while(num != 0)
            {
                index++;
                num /= 10;
            }

            skip_spaces(expr, &index);
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

static void skip_spaces(char* expr, int* index)
{
    while(isspace(expr[*index]))
        (*index)++;

    return;
}
