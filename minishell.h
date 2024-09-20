#ifndef MINISHELL_H
#define MINISHELL_H

#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
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
	Token   	**arguments;
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
	Token			**arg;
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
char	*handle_quote(char *str, char c);
// void	print_tokens(Token *tokens);

			//**libft**/
char	**ft_split(char *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strtrim(char *s1, char *set);

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

void print_queue(t_queue *queue);
void execute_ast(t_ast *root, char **envp);
#endif
