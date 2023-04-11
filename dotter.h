#ifndef DOTTER_H
#define DOTTER_H
#include <time.h>
#include <string.h>
#include "read_tree.h"
#include "log.h"


void graph_start();
void graph_add_dot(void* address, double value, int type, void* left, void* right, const char* fillcolor);
void graph_add_root(void* address);
void graph_add_arrow(void* address1, void* address2, const char* color);
void graph_end();
static char* get_dot_file_name();
static void close_log_file_dot();
static void print_graph();

void graph_start()
{
    static bool is_started = false;

    if(is_started == false)
    {
        is_started = true;
        fprintf(get_log_file(".dot"), "digraph {\n\n");
        //fprintf(get_log_file(".dot"), "rankdir=\"LR\";");
    }
}

void graph_add_dot(void* address, double value, int type, void* left, void* right, const char* fillcolor)
{
    char func_type[10] = "";
    func_to_str(type, func_type);

    if(equal_double(value, 14888841))
    {
        fprintf(get_log_file(".dot"), "node%p [shape = Mrecord, fillcolor = \"#FF0000\",style = filled, color = \"#000000\", label = \"{value = %lf | type = %d | address = %p | left = %p | right = %p}\"];\n", address, value, type, address, left, right);
        return;
    }

    if(type == NUMBER)
        fprintf(get_log_file(".dot"), "node%p [shape = Mrecord, fillcolor = \"%s\",style = filled, color = \"#000000\", label = \"{value = %lf | type = %d | address = %p | left = %p | right = %p}\"];\n", address, fillcolor, value, type, address, left, right);
    else if(type == VAR)
        fprintf(get_log_file(".dot"), "node%p [shape = Mrecord, fillcolor = \"%s\",style = filled, color = \"#000000\", label = \"{value = %c | type = var | address = %p | left = %p | right = %p}\"];\n", address, fillcolor, (char)value, address, left, right);
    else
        fprintf(get_log_file(".dot"), "node%p [shape = Mrecord, fillcolor = \"%s\",style = filled, color = \"#000000\", label = \"{type = %s | address = %p | left = %p | right = %p}\"];\n", address, fillcolor, func_type, address, left, right);
}

void graph_add_root(void* address)
{
    fprintf(get_log_file(".dot"), "root [style = filled, fillcolor = \"#D0FFD0\"]");
    fprintf(get_log_file(".dot"), "root -> node%p [color = \"#D0D0FF\"];\n", address);
}

void graph_add_arrow(void* address1, void* address2, const char* color)
{
    fprintf(get_log_file(".dot"), "node%p -> node%p [color = \"%s\"];\n", address1, address2, color);
}

void graph_end()
{
    static bool is_ended = false;
    if(is_ended == false)
    {
        is_ended = true;
    //atexit(close_log_file_dot);
        atexit(print_graph);
    }
}

static void print_graph()
{
    fprintf(get_log_file(".dot"), "\n}\n");
    close_log_file_dot();
    char cmd[200] = "";
    static char* dot_file_name = get_dot_file_name();

    sprintf(cmd, "dot %s -Tpng -o 3.png\n", dot_file_name);
    printf("%s", cmd);
    system(cmd);

    free(dot_file_name);
}


static char* get_dot_file_name()
{
    time_t timer = time(NULL);
    struct tm* time = localtime(&timer);
    char* filename = (char*)calloc(100, sizeof(char));

    sprintf(filename, ".log/%d_%d_%d_%d_%d_log.dot", time->tm_hour, time->tm_min, time->tm_mday,
            time->tm_mon+1, time->tm_year+1900);

   // printf("%s\n", filename);
    return filename;
}

void close_log_file_dot()
{
    FILE* fdot = get_log_file(".dot");
    fclose(fdot);
}

#endif
