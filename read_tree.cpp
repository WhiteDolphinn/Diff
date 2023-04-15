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
static void push_one_argument_func(char* expr, Node** root, int* index, char* source, char func_symbol, const char* func);
static void push_ln(char* expr, Node** root, int* index, char* func);
static void push_sin(char* expr, Node** root, int* index, char* func);
static void push_cos(char* expr, Node** root, int* index, char* func);
static void push_tan(char* expr, Node** root, int* index, char* func);
static void push_cot(char* expr, Node** root, int* index, char* func);

static int get_g(char* expr, Node** root);
static struct Node* get_n(char* expr, int* index);
static struct Node* get_e(char* expr, int* index);
static struct Node* get_t(char* expr, int* index);
static struct Node* get_p(char* expr, int* index);

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

    if(root == nullptr)
    {
        printf("root == nullptr\n");
        return;
    }

    if(!fscanf(source_file, " %[^\n]", expession))
    {
        printf("I can't read expession\n");
        return;
    }

    if(expession[MAX_STR_LENGTH - 1] != '\0')
    {
        printf("Переполение буфера expression\n");
        return;
    }

    read_node_inorder(expession, root);

    /*if(is_correct_read == false)
    {
        printf("Ошибка в чтении выражения.\n");
        delete_tree(*root);
    }*/
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
        is_correct_read = false;
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
        is_correct_read = false;
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

    char func[MAX_FUNC_LENGTH] = {};
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
    assert(expr != nullptr);
    assert(root != nullptr);
    assert(index != nullptr);
    assert(func != nullptr);
    return;
}

static void push_one_argument_func(char* expr, Node** root, int* index, char* source, char func_symbol, const char* func)
{
    if(stricmp(source, func))
        return;

    push_node(*root, func_symbol, func_symbol);

    for(size_t i = 0; i < strlen(func); i++)
        (*index)++;

    read_node_inorder(expr, &((*root)->left));
    (*root)->right = nullptr;

    skip_spaces(expr, index);
    return;
}

static void push_ln(char* expr, Node** root, int* index, char* func)
{
    push_one_argument_func(expr, root, index, func, LN, "ln");
    return;
}

static void push_sin(char* expr, Node** root, int* index, char* func)
{
    push_one_argument_func(expr, root, index, func, SIN, "sin");
    return;

}

static void push_cos(char* expr, Node** root, int* index, char* func)
{
    push_one_argument_func(expr, root, index, func, COS, "cos");
    return;
}


static void push_tan(char* expr, Node** root, int* index, char* func)
{
    push_one_argument_func(expr, root, index, func, TAN, "tan");
    return;
}

static void push_cot(char* expr, Node** root, int* index, char* func)
{
    push_one_argument_func(expr, root, index, func, COT, "cot");
    return;
}

void func_to_str(int type, char* func)
{
    #define DEFFUNC(SYMBOL, FUNC, PUSH, DIFF)   \
       if(SYMBOL == type)                       \
       {                                        \
            strcpy(func, #FUNC);                \
            return;                             \
       }                                        \

    #include "diff_funcs.h"
    #undef DEFFUNC

    strcpy(func, "unknown");
    return;
}

bool is_func(int type)
{
    #define DEFFUNC(SYMBOL, FUNC, PUSH, DIFF)   \
        if(SYMBOL == type)                      \
            return true;                        \

    #include "diff_funcs.h"
    #undef DEFFUNC

    return false;
}

int read_expession_rec_descent(FILE* source_file, Node** root)
{
    char expession_with_spaces[MAX_STR_LENGTH] = {};
    char expession_without_spaces[MAX_STR_LENGTH] = {};

    if(root == nullptr)
    {
        printf("root == nullptr\n");
        return ERROR_ROOT_POINTER;
    }

    if(!fscanf(source_file, " %[^\n]", expession_with_spaces))
    {
        printf("I can't read expession\n");
        return ERROR_READ_EXPESSION;
    }
    if(expession_with_spaces[MAX_STR_LENGTH - 1] != '\0')
    {
        printf("Переполение буфера expression\n");
        return ERROR_BUFFER_OVERFLOW;
    }

    int cur_exp_pos = 0;
    for(int i = 0; expession_with_spaces[i] != '\0'; i++)
        if(!isspace(expession_with_spaces[i]))
            expession_without_spaces[cur_exp_pos++] = expession_with_spaces[i];

    return get_g(expession_without_spaces, root);
}


static int get_g(char* expr, Node** root)
{
    int index = 0;
    *root  = get_e(expr, &index);

    if((*root)->type == SYNTAX_ERROR)
        return (int)(*root)->value;

    if(expr[index] != '\0')
    {
        printf("Syntax error in pos.%d. Symbol is %c\n", index, expr[index]);
        return SYNTAX_ERROR_IN_GET_G;
    }
    return 0;
}

static struct Node* get_n(char* expr, int* index)
{
    int value = 0;
    if(expr[*index] > '9' || expr[*index] < '0')
    {
        printf("Syntax error in pos.%d. Symbol is %c but expected number\n", *index, expr[*index]);
        return create_node(SYNTAX_ERROR, SYNTAX_ERROR_IN_GET_N);
    }

    while(expr[*index] <= '9' && expr[*index] >= '0')
    {
        int value2 = expr[*index] - '0';
        (*index)++;
        value = 10 * value + value2;
    }
    return create_node(NUMBER, value);
}

static struct Node* get_e(char* expr, int* index)
{
    struct Node* answer = get_t(expr, index);
    struct Node* cur_node = answer;

    while(expr[*index] == '+' || expr[*index] == '-')
    {
        int op = expr[*index];
        (*index)++;
        cur_node = create_node(op, op, answer);
        answer = cur_node;
        answer->right = get_t(expr, index);
    }
    return answer;
}

static struct Node* get_t(char* expr, int* index)
{
    struct Node* answer = get_p(expr, index);
    struct Node* cur_node = answer;

    while(expr[*index] == '*' || expr[*index] == '/')
    {
        int op = expr[*index];
        (*index)++;
        cur_node = create_node(op, op, answer);
        answer = cur_node;
        answer->right = get_p(expr, index);
    }
    return answer;
}

static struct Node* get_p(char* expr, int* index)
{
    if(expr[*index] == '(')
    {
        (*index)++;
        struct Node* answer = get_e(expr, index);

        if(expr[*index] != ')')
            printf("Syntax error in pos.%d. Symbol is %c but expected )", *index, expr[*index]);/////////////////////

        (*index)++;
        return answer;
    }

    return get_n(expr, index);
}
