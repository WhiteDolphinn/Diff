#include <ctype.h>
#include "read_tree.h"
#include "text.h"
#include "diff.h"

static void read_node_preorder(char* expr, Node** root);
static void read_node_inorder(char* expr, Node** root);
static void read_number_preorder(char* expr, Node** root, int* index);
static bool read_number_inorder(char* expr, Node** root, int* index);
static void read_sign_preorder(char* expr, Node** root, int* index);
static bool read_sign_inorder(char* expr, Node** root, int* index);
static void read_variable_inorder(char* expr, Node** root, int* index);

static void empty_func(char* expr, Node** root, int* index, char* func);
static void push_ln(char* expr, Node** root, int* index, char* func);
static void push_sin(char* expr, Node** root, int* index, char* func);
static void push_cos(char* expr, Node** root, int* index, char* func);

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

void skip_spaces(char* expr, int* index)
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

    void (*push_func)(char*, Node**, int*, char*) = empty_func;

    #define DEFFUNC(SYMB, FUNC, PUSH, DIFF) \
        push_func = PUSH;                   \
        push_func(expr, root, index, func); \

    #include "diff_funcs.h"

    #undef DEFFUNC


    skip_spaces(expr, index);
}

static void empty_func(char* expr, Node** root, int* index, char* func)
{
    return;
}

static void push_ln(char* expr, Node** root, int* index, char* func)
{
    if(stricmp(func, "ln"))
        return;

    push_node(*root, LN, LN);

    for(size_t i = 0; i < strlen(func); i++)
        (*index)++;

    read_node_inorder(expr, &((*root)->left));
    (*root)->right = nullptr;

    skip_spaces(expr, index);
    return;
}

static void push_sin(char* expr, Node** root, int* index, char* func)
{
    if(stricmp(func, "sin"))
        return;

    push_node(*root, SIN, SIN);

    for(size_t i = 0; i < strlen(func); i++)
        (*index)++;

    read_node_inorder(expr, &((*root)->left));
    (*root)->right = nullptr;

    skip_spaces(expr, index);
    return;
}

static void push_cos(char* expr, Node** root, int* index, char* func)
{
    if(stricmp(func, "cos"))
        return;

    push_node(*root, COS, COS);

    for(size_t i = 0; i < strlen(func); i++)
        (*index)++;

    read_node_inorder(expr, &((*root)->left));
    (*root)->right = nullptr;

    skip_spaces(expr, index);
    return;
}
