#ifndef TEXXER_H
#define TEXXER_H

#include <stdio.h>
#include "tree_soft.h"
#include "log.h"
#include "diff.h"

#define PRINT_U\
    add_eq_tex_without_head(node->left)\

#define PRINT_DIFF_U\
    fprintf(get_log_file(".tex"), "{(");\
    add_eq_tex_without_head(node->left);\
    fprintf(get_log_file(".tex"), ")}^{'}")\

#define PRINT_V\
    add_eq_tex_without_head(node->right)\


#define PRINT_DIFF_V\
    fprintf(get_log_file(".tex"), "{( ");\
    add_eq_tex_without_head(node->right);\
    fprintf(get_log_file(".tex"), " )}^{'}")\

#define PRINT_OP(OPERATOR)\
    fprintf(get_log_file(".tex"), "%s ", OPERATOR)\

#define PRINT_START_FRAC\
    fprintf(get_log_file(".tex"), "{(\\frac{ ")\

#define PRINT_MIDDLE_FRAC\
    fprintf(get_log_file(".tex"), " }{ ")\

#define PRINT_END_FRAC\
    fprintf(get_log_file(".tex"), " })} ")\

#define PRINT_OPEN_BRACKET\
    fprintf(get_log_file(".tex"), " {( ")\

#define PRINT_CLOSE_BRACKET\
    fprintf(get_log_file(".tex"), " )} ")\


#define PRINT_STRYKH\
    fprintf(get_log_file(".tex"), "^{'} ")\

#define PRINT_EQUALLY\
    fprintf(get_log_file(".tex"), "= ")\

#define PRINT_NUM(NUM)\
    fprintf(get_log_file(".tex"), "%d ", NUM)\

#define PRINT_LN_U\
    fprintf(get_log_file(".tex"), "ln(");\
    add_eq_tex_without_head(node->left);\
    fprintf(get_log_file(".tex"), ")")\

#define PRINT_LN_V\
    fprintf(get_log_file(".tex"), "ln(");\
    addeq_tex_without_head(node->right);\
    fprintf(get_log_file(".tex"), ")")\

#define PRINT_SIN_U\
    fprintf(get_log_file(".tex"), "sin(");\
    add_eq_tex_without_head(node->left);\
    fprintf(get_log_file(".tex"), ")")\


#define PRINT_COS_U\
    fprintf(get_log_file(".tex"), "cos(");\
    add_eq_tex_without_head(node->left);\
    fprintf(get_log_file(".tex"), ")")\


#define PRINT_TAN_U\
    fprintf(get_log_file(".tex"), "tan(");\
    add_eq_tex_without_head(node->left);\
    fprintf(get_log_file(".tex"), ")")\


#define PRINT_COT_U\
    fprintf(get_log_file(".tex"), "cot(");\
    add_eq_tex_without_head(node->left);\
    fprintf(get_log_file(".tex"), ")")\

void start_tex();
void end_tex();
void close_log_file_tex();
void add_eq_tex(struct Node* tree);
void add_eq_tex_without_head(struct Node* tree);
void add_diff_and_eq_tex(struct Node* eq, struct Node* diff);
void add_add_tex(struct Node* node);
void add_sub_tex(struct Node* node);
void add_mul_tex(struct Node* node);
void add_div_tex(struct Node* node);
void add_pow_x_a_tex(struct Node* node);
void add_pow_a_x_tex(struct Node* node);
void add_pow_f_g_tex(struct Node* node);
void add_ln_tex(struct Node* node);
void add_sin_tex(struct Node* node);
void add_cos_tex(struct Node* node);
void add_tan_tex(struct Node* node);
void add_cot_tex(struct Node* node);


void start_tex()
{
    static bool is_started = false;

    if(is_started == false)
    {
        is_started = true;
        fprintf(get_log_file(".tex"), "\n\\documentclass[a4paper, 12pt]{article}\n\\usepackage{graphicx} %% Required for inserting images\n\\usepackage[a4paper,top=1.3cm,bottom=2cm,left=1.5cm,right=1.5cm,marginparwidth=0.75cm]{geometry}\n\\usepackage{cmap}\n\\usepackage[T2A]{fontenc}\n\\usepackage[utf8]{inputenc}\n\\usepackage[english, russian]{babel}\n\n\\begin{document}\n\n");
        //fprintf(get_log_file(".tex"), "$");
    }
}

void end_tex()
{
    static bool is_ended = false;
    if(is_ended == false)
    {
        is_ended = true;
       // fprintf(get_log_file(".tex"), "$\n");
        fprintf(get_log_file(".tex"), "\\end{document}\n");
        atexit(close_log_file_tex);
    }
}

void close_log_file_tex()
{
    FILE* f = get_log_file(".tex");
    fclose(f);
}

void add_eq_tex_without_head(struct Node* tree)
{
    switch(tree->type)
    {
        case NUMBER:
        {
            fprintf(get_log_file(".tex"), "%lf", tree->value);
            break;
        }
        case VAR:
        {
            fprintf(get_log_file(".tex"), "%c", (char)tree->value);
            break;
        }

        case ADD: case SUB: case MUL: case DIV: case POW:
        {
            fprintf(get_log_file(".tex"), "{");
            //tree_print_inorder(get_log_file(".tex"), add_eq_tex_without_head(tree->left));
            add_eq_tex_without_head(tree->left);
            fprintf(get_log_file(".tex"), "}");

            if(tree->type != MUL)
                fprintf(get_log_file(".tex"), "%c", tree->type);
            else
                fprintf(get_log_file(".tex"), "\\cdot");

            fprintf(get_log_file(".tex"), "{");
            //tree_print_inorder(get_log_file(".tex"), add_eq_tex_without_head(tree->right));
            add_eq_tex_without_head(tree->right);
            fprintf(get_log_file(".tex"), "}");
            break;
        }

        case LN: case SIN: case COS: case TAN: case COT:
        {
            fprintf(get_log_file(".tex"), "{");
            switch(tree->type)
            {
                case LN: fprintf(get_log_file(".tex"), "ln(");   break;
                case SIN: fprintf(get_log_file(".tex"), "sin("); break;
                case COS: fprintf(get_log_file(".tex"),"cos("); break;
                case TAN: fprintf(get_log_file(".tex"), "tan("); break;
                case COT: fprintf(get_log_file(".tex"), "cot("); break;
                default: break;
            }

            tree_print_inorder(get_log_file(".tex"), tree->left);
            fprintf(get_log_file(".tex"), ")");
            fprintf(get_log_file(".tex"), "}");

            break;
        }
        default: break;
    }
}

void add_eq_tex(struct Node* tree)
{
    fprintf(get_log_file(".tex"), "\\begin{equation}\n");

    add_eq_tex_without_head(tree);

    fprintf(get_log_file(".tex"), "\n\\end{equation}\n\n");
}

void add_diff_and_eq_tex(struct Node* eq, struct Node* diff)
{
    fprintf(get_log_file(".tex"), "\\begin{equation}\n");

    fprintf(get_log_file(".tex"), "(");
    add_eq_tex_without_head(eq);
    fprintf(get_log_file(".tex"), ")^{'} = ");

    add_eq_tex_without_head(diff);

    fprintf(get_log_file(".tex"), "\n\\end{equation}\n\n");
}

void add_add_tex(struct Node* node)
{
    fprintf(get_log_file(".tex"), "ASS WEE can (know):");
    fprintf(get_log_file(".tex"), "\\begin{equation}\n");

    PRINT_OPEN_BRACKET;
        PRINT_U;
        PRINT_OP("+");
        PRINT_V;
    PRINT_CLOSE_BRACKET;
    PRINT_STRYKH;

    PRINT_EQUALLY;

    PRINT_DIFF_U;
    PRINT_OP("+");
    PRINT_DIFF_V;


    fprintf(get_log_file(".tex"), "\n\\end{equation}\n");
}

void add_sub_tex(struct Node* node)
{
    fprintf(get_log_file(".tex"), "Do you know the way? (this formula1):");
    fprintf(get_log_file(".tex"), "\\begin{equation}\n");

    PRINT_OPEN_BRACKET;
        PRINT_U;
        PRINT_OP("-");
        PRINT_V;
    PRINT_CLOSE_BRACKET;
    PRINT_STRYKH;

    PRINT_EQUALLY;

    PRINT_DIFF_U;
    PRINT_OP("-");
    PRINT_DIFF_V;

    fprintf(get_log_file(".tex"), "\n\\end{equation}\n");
}

void add_mul_tex(struct Node* node)
{
    fprintf(get_log_file(".tex"), "Важный CUMментарий:");
    fprintf(get_log_file(".tex"), "\\begin{equation}\n");

    PRINT_OPEN_BRACKET;
        PRINT_U;
        PRINT_OP("\\cdot");
        PRINT_V;
    PRINT_CLOSE_BRACKET;
    PRINT_STRYKH;

    PRINT_EQUALLY;

    PRINT_DIFF_U;
    PRINT_OP("\\cdot");
    PRINT_V;

    PRINT_OP("+");

    PRINT_DIFF_V;
    PRINT_OP("\\cdot");
    PRINT_U;

    fprintf(get_log_file(".tex"), "\n\\end{equation}\n");
}

void add_div_tex(struct Node* node)
{

    fprintf(get_log_file(".tex"), "Нетрудно заметить, что:");
    fprintf(get_log_file(".tex"), "\n\\begin{equation}\n");


    //PRINT_OPEN_BRACKET;
        PRINT_START_FRAC;
            PRINT_U;
        PRINT_MIDDLE_FRAC;
            PRINT_V;
        PRINT_END_FRAC;
    //PRINT_CLOSE_BRACKET;
    PRINT_STRYKH;

    PRINT_EQUALLY;

    PRINT_START_FRAC;
        PRINT_DIFF_U;
        PRINT_OP("\\cdot");
        PRINT_V;

        PRINT_OP("-");

        PRINT_DIFF_V;
        PRINT_OP("\\cdot");
        PRINT_U;
    PRINT_MIDDLE_FRAC;
        PRINT_V;
        PRINT_OP("\\cdot");
        PRINT_V;
    PRINT_END_FRAC;

    fprintf(get_log_file(".tex"), "\n\\end{equation}\n");
}

void add_pow_x_a_tex(struct Node* node)
{

    fprintf(get_log_file(".tex"), "Очевидно:");
    fprintf(get_log_file(".tex"), "\n\\begin{equation}\n");

    PRINT_OPEN_BRACKET;
        PRINT_U;
        PRINT_OP("^");
        PRINT_OPEN_BRACKET;
            PRINT_V;
        PRINT_CLOSE_BRACKET;
    PRINT_CLOSE_BRACKET;
    PRINT_STRYKH;

    PRINT_EQUALLY;

    PRINT_OPEN_BRACKET;
        PRINT_V;
        PRINT_OP("-");
        PRINT_NUM(1);
    PRINT_CLOSE_BRACKET;

    PRINT_OP("\\cdot");

    PRINT_OPEN_BRACKET;
        PRINT_U;
        PRINT_OP("^");

        PRINT_OPEN_BRACKET;
            PRINT_V;
            PRINT_OP("-");
            PRINT_NUM(1);
        PRINT_CLOSE_BRACKET;

    PRINT_CLOSE_BRACKET;

    PRINT_OP("\\cdot");

    PRINT_OPEN_BRACKET;
        PRINT_DIFF_U;
    PRINT_CLOSE_BRACKET;


    fprintf(get_log_file(".tex"), "\n\\end{equation}\n");
}

void add_pow_a_x_tex(struct Node* node)
{
    fprintf(get_log_file(".tex"), "ПОНятно, что:");
    fprintf(get_log_file(".tex"), "\n\\begin{equation}\n");

    PRINT_OPEN_BRACKET;
        PRINT_U;
        PRINT_OP("^");

        PRINT_OPEN_BRACKET;
            PRINT_V;
        PRINT_CLOSE_BRACKET;

    PRINT_CLOSE_BRACKET;
    PRINT_STRYKH;

    PRINT_EQUALLY;

    PRINT_LN_U;

    PRINT_OP("\\cdot");

    PRINT_OPEN_BRACKET;
        PRINT_U;
        PRINT_OP("^");

        PRINT_OPEN_BRACKET;
            PRINT_V;
        PRINT_CLOSE_BRACKET;
    PRINT_CLOSE_BRACKET;

    PRINT_OP("\\cdot");

    PRINT_OPEN_BRACKET;
        PRINT_DIFF_U;
    PRINT_CLOSE_BRACKET;


    fprintf(get_log_file(".tex"), "\n\\end{equation}\n");
}

void add_pow_f_g_tex(struct Node* node)
{
    fprintf(get_log_file(".tex"), "Даже ежу понятно, что:");
    fprintf(get_log_file(".tex"), "\n\\begin{equation}\n");

    PRINT_OPEN_BRACKET;
        PRINT_U;
        PRINT_OP("^");

        PRINT_OPEN_BRACKET;
            PRINT_V;
        PRINT_CLOSE_BRACKET;

    PRINT_CLOSE_BRACKET;
    PRINT_STRYKH;

    PRINT_EQUALLY;

    PRINT_OPEN_BRACKET;
        PRINT_U;
        PRINT_OP("^");

        PRINT_OPEN_BRACKET;
            PRINT_V;
        PRINT_CLOSE_BRACKET;

    PRINT_CLOSE_BRACKET;

    PRINT_OP("\\cdot");

    PRINT_OPEN_BRACKET;
        PRINT_DIFF_V;
        PRINT_LN_U;

        PRINT_OP("+");

        PRINT_START_FRAC;
            PRINT_V;
            PRINT_OP("\\cdot");
            PRINT_DIFF_U;
        PRINT_MIDDLE_FRAC;
            PRINT_U;
        PRINT_END_FRAC;

    PRINT_CLOSE_BRACKET;

    fprintf(get_log_file(".tex"), "\n\\end{equation}\n\n");
}

void add_ln_tex(struct Node* node)
{
    fprintf(get_log_file(".tex"), "Поймёт даже тот, кто не имеет отношения к математике:");
    fprintf(get_log_file(".tex"), "\n\\begin{equation}\n");

    PRINT_OPEN_BRACKET;
        PRINT_LN_U;
    PRINT_CLOSE_BRACKET;
    PRINT_STRYKH;

    PRINT_EQUALLY;

    PRINT_START_FRAC;
        PRINT_DIFF_U;
    PRINT_MIDDLE_FRAC;
        PRINT_U;
    PRINT_END_FRAC;

    fprintf(get_log_file(".tex"), "\n\\end{equation}\n\n");

}

void add_sin_tex(struct Node* node)
{
    fprintf(get_log_file(".tex"), "WEEдно, что:");
    fprintf(get_log_file(".tex"), "\n\\begin{equation}\n");

    PRINT_OPEN_BRACKET;
        PRINT_SIN_U;
    PRINT_CLOSE_BRACKET;
    PRINT_STRYKH;

    PRINT_EQUALLY;

    PRINT_OPEN_BRACKET;
        PRINT_DIFF_U;
    PRINT_CLOSE_BRACKET;

    PRINT_OP("\\cdot");

    PRINT_OPEN_BRACKET;
        PRINT_COS_U;
    PRINT_CLOSE_BRACKET;

    fprintf(get_log_file(".tex"), "\n\\end{equation}\n\n");

}

void add_cos_tex(struct Node* node)
{
    fprintf(get_log_file(".tex"), "WEEдно, что:");
    fprintf(get_log_file(".tex"), "\n\\begin{equation}\n");

    PRINT_OPEN_BRACKET;
        PRINT_COS_U;
    PRINT_CLOSE_BRACKET;
    PRINT_STRYKH;

    PRINT_EQUALLY;

    PRINT_OPEN_BRACKET;
        PRINT_OP("-");
        PRINT_DIFF_U;
    PRINT_CLOSE_BRACKET;

    PRINT_OP("\\cdot");

    PRINT_OPEN_BRACKET;
        PRINT_SIN_U;
    PRINT_CLOSE_BRACKET;

    fprintf(get_log_file(".tex"), "\n\\end{equation}\n\n");

}

void add_tan_tex(struct Node* node)
{
    fprintf(get_log_file(".tex"), "WEEдно, что:");
    fprintf(get_log_file(".tex"), "\n\\begin{equation}\n");

    PRINT_OPEN_BRACKET;
        PRINT_TAN_U;
    PRINT_CLOSE_BRACKET;
    PRINT_STRYKH;

    PRINT_EQUALLY;

    PRINT_START_FRAC;
        PRINT_DIFF_U;
    PRINT_MIDDLE_FRAC;
        PRINT_COS_U;
        PRINT_OP("\\cdot");
        PRINT_COS_U;
    PRINT_END_FRAC;

    fprintf(get_log_file(".tex"), "\n\\end{equation}\n\n");

}

void add_cot_tex(struct Node* node)
{
    fprintf(get_log_file(".tex"), "WEEдно, что:");
    fprintf(get_log_file(".tex"), "\n\\begin{equation}\n");

    PRINT_OPEN_BRACKET;
        PRINT_COT_U;
    PRINT_CLOSE_BRACKET;
    PRINT_STRYKH;

    PRINT_EQUALLY;

    PRINT_START_FRAC;
        PRINT_OP("-");
        PRINT_DIFF_U;
    PRINT_MIDDLE_FRAC;
        PRINT_SIN_U;
        PRINT_OP("\\cdot");
        PRINT_SIN_U;
    PRINT_END_FRAC;

    fprintf(get_log_file(".tex"), "\n\\end{equation}\n\n");

}

#endif
