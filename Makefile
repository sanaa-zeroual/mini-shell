CFLAGS = -Wall -Wextra -Werror
CC = cc
LFLAGS = -lreadline
NAME = minishell

SRCS =  mnsh.c parsing/main.c parsing/analyse_tokens.c parsing/expand.c parsing/analyse_tokens.c \
		parsing/token_into_stack.c parsing/signal_handle.c \
		parsing/postfix_to_tree.c parsing/minishell_utils.c \
		parsing/tokenization.c parsing/infix_to_postfix.c \
		parsing/pipex_utils.c parsing/parser.c \
		execution/builtins/cd/cd.c execution/builtins/echo/echo.c execution/builtins/env/env.c \
		execution/builtins/exit/exit.c execution/builtins/pwd/pwd.c execution/builtins/unset/unset.c \
		execution/builtins/builtins.c execution/exec/commands.c execution/exec/ft_utiles.c \
		libft/*.c
OBJS = $(SRCS:.c=.o)execution/libftt/*.c 

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: clean all