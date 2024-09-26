# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror

# Source directories
SRC_DIR = parsing execution libftt
OBJ_DIR = objs
BIN = minishell

# Source files
PARSE_SRCS = $(wildcard parsing/*.c)
EXEC_SRCS = $(wildcard execution/*.c)
LIBFT_SRCS = $(wildcard libftt/*.c)

# Object files
PARSE_OBJS = $(PARSE_SRCS:.c=.o)
EXEC_OBJS = $(EXEC_SRCS:.c=.o)
LIBFT_OBJS = $(LIBFT_SRCS:.c=.o)

# Combine all object files
OBJ = $(PARSE_OBJS) $(EXEC_OBJS) $(LIBFT_OBJS)

# Rule to create the final executable
$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# General rule to compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf $(OBJ)

# Remove the executable
fclean: clean
	rm -rf $(BIN)

# Rebuild everything
re: fclean $(BIN)

.PHONY: clean fclean re
