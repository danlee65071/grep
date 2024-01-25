#include "s21_grep.h"

void error(const char *mesg, int errnum)
{
    if (errnum)
        fprintf(stderr, "s21_grep: %s: %s\n", mesg, strerror(errnum));
    else
        fprintf(stderr, "s21_grep: %s\n", mesg);
}

void fatal(const char *mesg, int errnum)
{
    error(mesg, errnum);
    exit(2);
}

bool get_flag_val(t_flag *flags, const char key)
{
    for (int i = 0; i < NUM_FLAGS; i++)
    {
        if (flags[i].flag == key)
            return flags[i].is_exist;
    }
    return false;
}

void set_flag_val(t_flag **flags, const char key, const bool val)
{
    for (int i = 0; i < NUM_FLAGS; i++)
    {
        if ((*flags)[i].flag == key)
        {
            (*flags)[i].is_exist = val;
            return;
        }
    }
}

char* strjoin(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    if (!result)
        fatal("memory exhausted", 0);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

void* extended_realloc(void* ptr, size_t size)
{
    void *result;
    if (ptr)
        result = realloc(ptr, size);
    else
        result = malloc(size);
    if (size && !result)
        fatal("memory exhausted", 0);
    return result;
}

FILE *open_file(const char *filename, const char *mode, t_flag *flags)
{
    FILE *file = fopen(filename, mode);
    if (!file)
    {
        if (!flags || !get_flag_val(flags, 's'))
            fatal(filename, 2);
        exit(2);
    }
    return file;
}

void output_line(char *line, int n)
{
    for (int i = 0; i < n; i++)
        putchar(line[i]);
    if (line[n - 1] != '\n')
        putchar('\n');
}

void free_line(char** line)
{
    if (line && *line)
    {
        free(*line);
        *line = NULL;
    }   
}
