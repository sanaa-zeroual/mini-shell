#ifndef MINISHELL_H
#define MINISHELL_H

#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include "parsing/gnl/get_next_line.h"
#include "execution/libftt/libft.h"
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <limits.h>
#include <signal.h>
#include <unistd.h>

#define PREC_WHITESPACES          25  
#define PREC_BACKTICK             24
#define PREC_DOLLAR               23
#define PREC_TILDLE               22 
#define PREC_PARENTHESES          21 
#define PREC_LOGICAL_NOT          20 
#define PREC_ASTERISK             19 
#define PREC_SLASH                18
#define PREC_BACKSLASH            17 
#define PREC_QUESTION             16
#define PREC_DOT                  15
#define PREC_ADDITIVE             14
#define PREC_ASSIGNMENT           13
#define PREC_AMPERSAND            12
#define PREC_LOGICAL_AND          11 
#define PREC_PIPE                 9  // LOWER than redirection
#define PREC_LOGICAL_OR           8
#define PREC_REDIR_IN             10  // HIGHER than pipe
#define PREC_REDIR_OUT            10  // HIGHER than pipe
#define PREC_REDIR_APPEND         10  // HIGHER than pipe
#define PREC_REDIR_HERE_DOC       10  // HIGHER than pipe
#define PREC_SEMICOLON            4
#define PREC_COLON                3 
#define PREC_NEW_LINE             2


typedef enum
{
	TOKEN_DOUBLE_AMP,     // "&&"
	TOKEN_TILDLE,         // "~"
	TOKEN_PIPE,           // "|"
	TOKEN_DOUBLE_PIPE,    // "||"
	TOKEN_REDIR_IN,       //"<"
	TOKEN_REDIR_OUT,      //">"
	TOKEN_REDIR_APPEND,   // ">>"
	TOKEN_REDIR_HERE_DOC, // "<<"
	TOKEN_DOLLAR,         // "$"
	TOKEN_OPEN_PARENTH,   // "( , [  , {"
	TOKEN_CLOSE_PARENTH,  // ") , ] , ]"
	TOKEN_COMMAND,
	TOKEN_OPTION,
	TOKEN_DOUBLE_QUOTED,
	TOKEN_BUILT_IN,
	TOKEN_SINGLE_QUOTED,
	TOKEN_ARGUMENT, // String
	TOKEN_UNKNOWN
}					TokenType;

typedef struct token
{
	TokenType		type;
	char			*value;
	char			**expanded_value;
	struct token	*next;
	struct token	*previous;
}					Token;

typedef enum 
{
	COMMAND,
	PIPELINE,
	REDERECTION_IN,
	REDERECTION_OUT,
	REDERECTION_APPEND,
	REDERECTION_HEREDOC,
	REDERECTION_SEMICOLON,
}AST_TYPE;

typedef struct parse
{
	Token		*token;
	char		**arguments;
	int         input_fd;
	int         output_fd;
	struct parse *next;
}t_parser;


typedef struct s_ast
{
	t_parser		*data;
	AST_TYPE		type;
	struct s_ast	*left;
	struct s_ast	*right;
	struct s_ast	*next;
}					t_ast;

typedef struct queue
{
	Token			*node;
	char				**arg;
	struct queue	*next;
}					t_queue;

typedef struct stack
{
	t_parser			*node;   // Changed to t_ast pointer
	struct stack		*next;
}					t_stack;

			//**Tokenization**/
Token	**tokenize(char *input);
void	add_token(Token **tokens, TokenType type, const char *value);
char *handle_quote(char *str);

// void	print_tokens(Token *tokens);

			//**libft**/
// char	**ft_split(char *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);
// char	*ft_strtrim(char *s1, char *set);

		    //pipex_utils
char	*get_executable(char *command);

			//generate_postfix
t_queue *generate_postfix(t_parser *tokens);
int		get_precedence(int token_type);
void	transfer_tokens_to_stack(t_parser *token_list, t_stack **stack);
int		check_precedence(t_stack *stack, int token_type);
void	push_back_stack(t_stack **src, t_stack **dest);

			//abstract syntax tree
t_ast	*generate_ast_from_postfix(t_queue *postfix_output);
t_stack *pop_stack(t_stack **stack);
t_ast	*pop_ast_stack(t_ast **ast_stack);
int		is_operator(Token *node);
int		is_operand(Token *node);
t_ast	*push_to_ast_stack(t_ast *ast_stack, t_ast *ast_node);

			//mini_utils
t_stack	*new_stack_node(t_parser *token);
int		check_syntax_errors(Token *tokens);
char	quote_type(const char *str);
Token	*create_token(TokenType type, const char *value);
char	*char_to_string(char c, char c2);
int		get_token_type(const char *token, char c);
void print_tokens(t_parser *tokens);

			//signals
void handle_signal();
			//expand
void expand(Token *tokens);

				//analyse_tokens
t_parser *analyse_tokens(Token **tokens);

				//signals
void handle_ctrl_c();
void handle_ctrl_d();

Token *get_last_token(Token *token);

// void print_queue(t_queue *queue);
int	ft_counter(char *str, char c);
int ft_is_separator(char c);
void print_ast(t_ast *ast, int depth);


///////////////////// execution /////////////////////////

typedef struct s_env
{
	char					*name;
	char					*vale;
	struct s_env			*next;
    struct s_env             *prv;
}							t_envi;

typedef struct  s_pipe
{
	int write_end;
	int read_end;
}						t_pipe;

// typedef struct  s_exec
// {
//     char **args;
//     char

// }       t_exec;

//exit 
typedef struct s_shell
{
    int exit_status;
    char **args;
} t_shell;

typedef struct s_mini
{
    t_envi *env;
    t_shell *shell;
    char **ptr;
    char **arr;
}t_mini;

		//redirections

int redir_fd_in(t_ast *cmd);
int redir_fd_out(t_ast *cmd);

		//pipeline

pid_t right_pipe(t_ast *cmd, t_pipe *pipe_fds, t_mini *box);
pid_t left_pipe(t_ast *cmd, t_pipe *pipe_fds, t_mini *box);
void execute_pipeline(t_ast *cmd, t_mini *box);

		//builtins
		
int builtins(char **av, t_mini *box);
int is_builtin(char *cmd);
void ft_putstr_fd(char *str, int fd);
int	ft_cd(char **ptr, t_envi *envi);
void	update_env(t_envi *envi);
t_envi	*search_env(t_envi *envi, char *name);
int is_n_option(char *arg);
int first_non_option(char **args);
int ft_echo(char **args);
int	ft_unset(char **ptr, t_mini *box);
void ft_remove(t_mini *box);
int	f__plus(char *r);
int	ft_export(char **ptr, t_envi *env);
int ft_pwd( t_envi *env);
int	ft_exit(t_shell *shell);
int ft_env(t_envi *env);

		//extenal command
char **separate_env(t_envi *env);
char **get_command(t_ast *cmd);
char **get_path();
int count_arguments(char **arguments);
void executing(t_ast *node, t_mini *box);
void algo_execution(t_ast *cmd, t_mini *box);
int handle_redir(t_ast *root, t_mini *box);
int manage_io(t_ast *root);
int handle_hd(char *delim, int expand, t_mini *box);
char *expand_doc(char *s, t_mini *box);
ssize_t calc_len(char *s, t_mini *box);
void exec_cmd(t_ast *cmd, t_mini *box);
int rfd_out(t_ast *cmd);
char *expand_var(char *s, ssize_t *i, t_mini *box);
int rfd_in(t_ast *cmd);
void algo_execution(t_ast *cmd, t_mini *box);
int is_identifier(int c);
int is_identifier(int c);
void exec_command_with_redirection(t_ast *cmd);
#endif