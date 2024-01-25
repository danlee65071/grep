#include "s21_grep.h"

void add_template(t_grep *information, char* template);
void parse_filenames(int argc, char** argv, t_grep* information);
char* parse_file_with_templates(const char* filename);
void combo_check(t_flag *flags, char *path, int c);

void parse_options(int argc, char **argv, t_flag **flags, t_grep *information)
{
    int opt = 0;
    char *template;

    while ((opt = getopt_long(argc, argv, "e:ivclnhsf:o", 0, 0)) != EOF)
    {
        if (opt == '?')
            exit(2);
        set_flag_val(flags, opt, true);
        if (optarg)
        {
            if (opt == 'f')
                template = parse_file_with_templates(optarg);
            else
                template = strdup(optarg);
            add_template(information, template);
            free_line(&template);
        }
        information->num_flags++;
    }
    if (!get_flag_val(*flags, 'e') && !get_flag_val(*flags, 'f'))
        information->templates = strdup(argv[optind++]);
    if (!information->num_flags)
        add_template(information, *(argv + optind++));
    if (argc - optind == 1)
        set_flag_val(flags, 'h', true);
    parse_filenames(argc, argv,information);
    return;
}

void add_template(t_grep *information, char* template)
{
    char *tmp = NULL;

    if (!information->templates)
        information->templates = strdup(template);
    else
    {
        tmp = information->templates;
        information->templates = strjoin(information->templates, "|");
        free_line(&tmp);
        tmp = information->templates;
        information->templates = strjoin(information->templates, template);
        free_line(&tmp);
    }
}

void parse_filenames(int argc, char **argv, t_grep *information)
{
    for (int i = 0; i + optind < argc; i++)
    {
        // information->filenames = extended_realloc(information->filenames, sizeof(char *) * (information->num_filenames + 1));
        // information->filenames[i] = strdup(argv[i + optind]);
        information->filenames = extended_realloc(information->filenames, sizeof(char *) * (information->num_filenames + 1));
        *(information->filenames + i) = strdup(*(argv + i + optind));
        information->num_filenames++;
    }
}

void output_match(regex_t *reg, char *line)
{
    regmatch_t match;
    int offset = 0;
    while (1)
    {
        // printf("line:\n%s", line + offset);
        int result = regexec(reg, line + offset, 1, &match, REG_NOTBOL);
        // printf("%d\n", result);
        if (result)
            break;
        // printf("so: %lld\n", match.rm_so);
        // printf("eo: %lld\n", match.rm_eo);
        for (int i = match.rm_so; i < match.rm_eo; i++)
            putchar(*(line + i + offset));
        putchar('\n');
        offset += match.rm_eo;
    }
}

void process_files(t_flag *flags, t_grep information)
{
    regex_t regex;
    int i_case = get_flag_val(flags, 'i') ? REG_ICASE : 0;
    
    printf("%s\n", information.templates);
    if (regcomp(&regex, information.templates, REG_EXTENDED | i_case))
        perror("Error");
    for (size_t i = 0; i < information.num_filenames; i++)
    {
        int c = 0;
        t_getline line = {0};
        line.file = open_file(*(information.filenames + i), "r", flags);
        while ((line.read = getline(&line.line, &line.len, line.file)) != -1)
        {
            int result = regexec(&regex, line.line, 0, NULL, 0);
            if ((!result && !get_flag_val(flags, 'v')) || (get_flag_val(flags, 'v') && result))
            {
                if (!get_flag_val(flags, 'c') && !get_flag_val(flags, 'l'))
                {
                    if (!get_flag_val(flags, 'h'))
                        printf("%s:", *(information.filenames + i));
                    if (get_flag_val(flags, 'n'))
                        printf("%zu:", line.line_count+1);
                    if (get_flag_val(flags, 'o') && !get_flag_val(flags, 'v'))
                        output_match(&regex, line.line);
                    else
                        output_line(line.line, line.read);
                }
                c++;
            }
            line.line_count++;
        }
        combo_check(flags, *(information.filenames + i), c);
        free_line(&(line.line));
        fclose(line.file);
    }
    regfree(&regex);
}


char* parse_file_with_templates(const char* filename)
{
    char* templates = NULL;
    char* tmp; 
    char* buf;
    t_getline line = {0};
    line.file = open_file(filename, "r", NULL);
    while ((line.read = getline(&line.line, &line.len, line.file)) != -1)
    {
        if (*(line.line + line.read - 1) == '\n')
            buf = substr(line.line, 0, line.read - 1);
        else
            buf = strdup(line.line);
        
        if (!templates)
            templates = strdup(buf);
        else
        {
            if (strlen(buf) > 0)
            {
                tmp = templates;
                templates = strjoin(templates, "|");
                free_line(&tmp);
            }
            tmp = templates;
            templates = strjoin(templates, buf);
            free_line(&tmp);
        }
        free_line(&(line.line));
        free_line(&buf);
    }
    free_line(&(line.line));
    fclose(line.file);
    return templates;
}

void combo_check(t_flag *flags, char *path, int c)
{
    if (get_flag_val(flags, 'c') && !get_flag_val(flags, 'l'))
    {
        if (!get_flag_val(flags, 'h'))
            printf("%s:", path);
        printf("%d\n", c);
    }
    if (get_flag_val(flags, 'v'))
    {
        if (get_flag_val(flags, 'l') && get_flag_val(flags, 'c'))
        {
            if (!get_flag_val(flags, 'h'))
                printf("%s:1\n", path);
            else
                printf("1\n");
        }
    }
    if (get_flag_val(flags, 'l') && get_flag_val(flags, 'c') && !get_flag_val(flags, 'v'))
    {
        if (!get_flag_val(flags, 'h'))
            printf("%s:", path);
        if (c < 1)
            printf("0\n");
        if (c > 0)
            printf("1\n");
    }
    if (get_flag_val(flags, 'l') && c > 0)
        printf("%s\n", path);
}
