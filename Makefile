CFLAGS = -Wall -Wextra -Werror
CC = cc
LFLAGS = -lreadline
NAME = minishell

# Use wildcard to gather all .c files from libft and libftt directories
SRCS =  mnsh.c \
        parsing/main.c \
        parsing/analyse_tokens.c \
        parsing/expand.c \
        parsing/token_into_stack.c \
        parsing/signal_handle.c \
        parsing/postfix_to_tree.c \
        parsing/minishell_utils.c \
        parsing/tokenization.c \
        parsing/infix_to_postfix.c \
        parsing/pipex_utils.c \
        parsing/parser.c \
        execution/builtins/cd/cd.c \
        execution/builtins/echo/echo.c \
        execution/builtins/env/env.c \
        execution/builtins/exit/exit.c \
        execution/builtins/pwd/pwd.c \
        execution/builtins/unset/unset.c \
        execution/builtins/builtins.c \
		execution/builtins/export/export.c \
        execution/handle_command.c \
        execution/handle_pipe.c \
        execution/handle.redirection.c \
        execution/utils.c \
        execution/postorde.c \
        $(wildcard libft/*.c) \
        $(wildcard execution/libftt/*.c)

# Generate object file names from source file names
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all
