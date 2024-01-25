NAME = s21_grep
SRCS = s21_grep.c utils.c process.c
SRCS_DIR = src
HEADERS = s21_grep.h
HEADERS_DIR = includes
PATH_HEADERS = $(addprefix $(HEADERS_DIR)/, $(HEADERS))
OBJS = $(patsubst %.c, %.o, $(SRCS))
DIR_OBJS = obj
PATH_OBJS = $(addprefix $(DIR_OBJS)/, $(OBJS))
CC = gcc
FLAGS = -Wall -Werror -Wextra -g -std=c11
RM = rm -rf


all: $(NAME)

$(NAME): $(PATH_OBJS)
	$(CC) $(FLAGS) -I $(HEADERS_DIR) $^ -o $@
	
$(DIR_OBJS)/%.o: $(SRCS_DIR)/%.c $(PATH_HEADERS) Makefile
	mkdir -p $(DIR_OBJS)
	$(CC) $(FLAGS) -I $(HEADERS_DIR) -c $< -o $@

clean:
	$(RM) $(DIR_OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
