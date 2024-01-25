#ifndef S21_GREP
# define S21_GREP

# define NUM_FLAGS 10

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <getopt.h>
# include <regex.h>
# include <stdbool.h>


typedef struct s_flag
{
    char flag;
    bool is_exist;
} t_flag;


typedef struct s_grep
{
    char* flags;
    char* templates;
    size_t num_flags;
    size_t num_filenames;
    char** filenames;
} t_grep;


typedef struct s_getline
{
    FILE *file;
    char *line;
    size_t len;
    ssize_t read;
    size_t line_count;
} t_getline;


// utils
void *extended_realloc(void *ptr, size_t size);
void fatal(const char *mesg, int errnum);
bool get_flag_val(t_flag *flags, const char key);
void set_flag_val(t_flag** flags, const char key, const bool val);
char* strjoin(const char* s1, const char* s2);
FILE* open_file(const char* filename, const char* mode, t_flag* flags);
void output_line(char *line, int n);
void free_line(char** line);

// process
void parse_options(int argc, char **argv, t_flag** flags, t_grep *information);
void process_files(t_flag *flags, t_grep information);


#endif