#include "s21_grep.h"


t_flag* init_flags(t_grep information)
{
    t_flag* flags = extended_realloc(NULL, sizeof(t_flag) * NUM_FLAGS);
    for (int i = 0; i < NUM_FLAGS; i++)
    {
        // flags[i].flag = information.flags[i];
        (*(flags + i)).flag = *(information.flags + i);
        (*(flags + i)).is_exist = false;
    }
    return flags;
}

void execute(int argc, char **argv)
{
    t_grep information = {0};
    information.flags = "eivclnhsfo";
    t_flag* flags = init_flags(information);
    parse_options(argc, argv, &flags, &information); 
    process_files(flags, information);
    free(information.templates);
    free(flags);
    for (size_t i = 0; i < information.num_filenames; i++)
        free(*(information.filenames + i));
    free(information.filenames);
}

int main(int argc, char **argv)
{
    execute(argc, argv);
    return 0;
}
