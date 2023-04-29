#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include "text.h"
#include "log.h"


size_t num_of_symbols(const char* name_of_file)
{
    struct stat st = {};
    if (stat(name_of_file, &st) == -1)
        return 0;

    return (size_t)st.st_size;
}

char* text_reader(FILE* file, const char* name_of_file)
{
    size_t SIZE = num_of_symbols(name_of_file);
    if(SIZE == 0)
    {
        printf("Error in num_of_symbols()\n");
        return nullptr;
    }

    char* text = (char*)calloc(SIZE+1, sizeof(char));

    if(text == nullptr)
    {
        printf("I can't read this file(((\n");
        return nullptr;
    }
    size_t size_text = fread(text, sizeof(char), SIZE, file);

    char* temp = (char*)realloc(text, sizeof(char) * size_text + 1);

    if(temp == nullptr)
    {
        printf("I can't read this text((((\n");
        return nullptr;
    }

    text = temp;
    return text;
}

int stricmp(const char* source1, const char* source2)
{
    char lower_source1[MAX_STR_LENGTH] = "";
    for(int i = 0; source1[i]; i++)
        lower_source1[i] = (char)tolower(source1[i]);

    char lower_source2[MAX_STR_LENGTH] = "";
    for(int i = 0; source2[i]; i++)
        lower_source2[i] = (char)tolower(source2[i]);

    return strcmp(lower_source1, lower_source2);
}
