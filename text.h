#ifndef TEXT_H
#define TEXT_H
#include <stdio.h>

#define MAX_STR_LENGTH 300

size_t num_of_symbols(const char* name_of_file);
char* text_reader(FILE* file, const char* name_of_file);

#endif
