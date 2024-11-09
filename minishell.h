/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:39:23 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/07 14:47:43 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "externel_folder/gnl/get_next_line.h"
# include "externel_folder/libftt/libft.h"
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
#include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
#include <errno.h>
# define SIGINT 2 
# define SIGQUIT 3
# define SIGTERM 15

// typedef struct s_mini
// {
//     t_envi *env;
//     t_shell *shell;
//     char **ptr;
//     char **arr;
// }t_mini;

typedef enum
{
	TOKEN_TILDLE,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_DOUBLE_QUOTED,
	TOKEN_SINGLE_QUOTED,
	TOKEN_OPEN_PARENTH,
	TOKEN_CLOSE_PARENTH,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HERE_DOC,
	TOKEN_COMMAND,
	DELIMITER,
	TOKEN_OPTION,
	TOKEN_BUILT_IN,
	TOKEN_ARGUMENT,
	TOKEN_UNKNOWN
}
					TokenType;
typedef enum
{
	PIPE,
	RE_OUT,
	RE_IN,
	RE_HEREDOC,
	RE_APPEND,
	UNKOWN
}					t_type;
typedef struct s_shell
{
	int				exit_status;
	char			**args;
}					t_shell;

typedef struct s_env
{
	char			*name;
	char			*vale;
	struct s_env	*next;
	struct s_env	*prv;
}					t_envi;


typedef struct s_mini
{
	t_envi			*env;
	t_shell			*shell;
	char			**ptr;
	char			**arr;
	int				last_exit_status;
}					t_mini;
typedef struct token
{
	TokenType					type;
	char						*value;
	char						**expanded_value;
	struct token				*next;
	struct token				*previous;
}								t_token;

typedef struct s_file
{
    char *filename;
    int type;
    char *red; // The path of the redirection
    struct s_file *next;
} t_file;

typedef struct s_cmd
{
    t_type type;
    char **arguments;
    t_file *file;
    struct s_cmd *prev;
    struct s_cmd *next;
    char *cmd_path; // The path of the command
    int fd_in;
    int fd_out;
    int pipe_fd[2];
    int pid;
} t_cmd;

typedef struct s_var
{
    int exit_status; // The exit status
    t_mini *box;     // Need to access env
    int out_fd;
    int red_error;
    int pre_pipe_infd;
    int last_child_id;
    char **envp;
    int size;
} t_var;

extern t_var g_var; // Declare the global variable

typedef struct garbage_collector
{
	void						*ptr;
	struct garbage_collector	*next;
}								t_gc;

typedef struct global
{
	int 	exit_status;
	t_gc	*head;
}globalvar;

extern globalvar var;

t_token							**tokenize(char *input);
char							*handle_quote(char *str);
int								is_special(char c);
int								dollar_counter(char *input);
char							*single_quote_expansion(char *input, int *i);
char							*double_quote_expansion(char *input, int *i);
int								is_quoted(char *input);
char							*expand_non_operator(char *token);
int								built_in_checker(const char *str);
void							add_token(t_token **tokens, TokenType type,
									char *value, int *k);
char							*get_executable(char *command);
char							*get_inside_quote(char *tmp, int *i, int *j);
char							*process_delimiter(char *tmp);
void							handle_heredoc(t_token **tokens, char *input,
									int *i);
void							heredoc_process(t_cmd **node, t_file **head,
									t_token **tokens);
char							*tidle_expansion(int *i);
void							fill_up_node(t_cmd **node, t_token **tokens, t_file *head);
char							*dollar_expand(char *input, int *i);
void							go_to_next(t_token **tokens);
char							**result_traitement(char *input);
char							*get_string(char *input, int *i);
int								get_size(char **arr);
int								get_size_arr(char *input);
char							*parse_line(char *input);
char							**handle_that_shit(char *input);
char							**unquoted_result(char **input);
char							*get_word_to_expand(char *str, int *j);
void							add_quote(char *input, char **expanded_value,
									int *j);
int								is_operator(t_token *node);
int								is_operand(t_token *node);
int								handle_consecutive_operator(t_token *tokens);
int								handle_paren(t_token *token);
int								handle_quotes(t_token *tokens);
int								check_token(char *str, char c);
t_token							*get_last_token(t_token *token);
int								handle_operators_bg_en(t_token *tokens);
int								check_syntax_errors(t_token *tokens);
char							quote_type(const char *str);
char							*char_to_string(char c, char c2);
int								get_token_type(const char *token, char c);
void							handle_signal(void);
void							handle_ctrl_d(void);
void							handle_ctrl_c(void);
int								expand(t_token *tokens);
t_cmd							*analyse_tokens(t_token **tokens);
void							handle_ctrl_c(void);
void							handle_ctrl_d(void);
int								ft_is_separator(char c);
void							print_cmd(t_cmd *cmd);
int								is_red(t_token *token);
int								get_red_type(t_token *token);
int								nbr_argument(t_token *tokens);
void							push_back(t_cmd **lst, t_cmd *node);
void							push_t_file(t_file **head, t_file *node);
void							*ft_malloc(size_t size, int ele_nbr);
void							clean_gc(void);

///////////////////// execution /////////////////////////


// int					builtins(char **av, t_mini *box);
int					is_builtin(char *cmd);
void				ft_putstr_fd(char *str, int fd);
int					ft_cd(char **ptr, t_envi *envi);
void				update_env(t_envi *envi);
t_envi				*search_env(t_envi *envi, char *name);
int					is_n_option(char *arg);
int					first_non_option(char **args);
int					ft_echo(char **args);
int					ft_unset(char **ptr, t_mini *box);
void				ft_remove(t_mini *box);
int					f__plus(char *r);
int					ft_export(char **ptr, t_envi *env);
int					ft_pwd(t_envi *env);
int					ft_exit(t_shell *shell);
int					ft_env(t_envi *env);
/////////////////////////////builtins///////////////////////////

int	ft_cd(char **ptr, t_envi *envi);
void	update_env(t_envi *envi);
t_envi	*search_env(t_envi *envi, char *name);
int ft_echo(char **args);
int first_non_option(char **args);
int is_n_option(char *arg);
int	ft_export(char **ptr, t_envi *env);
int	add_one(char **ptr, t_envi *env);
int	process_single_env(char *ptr_i, t_envi *env);
int	process_single_env(char *ptr_i, t_envi *env);
int	process_single_env(char *ptr_i, t_envi *env);
int	ft_unset(char **ptr, t_mini *box);
void	ft_remove(t_mini *box);
// int	ft_pwd(t_envi *env);
int	ft_exit(t_shell *shell);
int	ft_env(t_envi *env);


// extenal command

char				**separate_env(t_envi *env);
char				**get_path(void);
int					count_arguments(char **arguments);

void execute_pipe(t_cmd *cmd, t_mini *box);


////////////////////////////////new shell////////////////////////////////
void validate_cmd(t_cmd *cmd);
char *allocate_folders(char *path, int i);
void check_cmd_path(t_cmd *cmd);
char *r_quotes(char *str);
void my_strncpy(char *dest, const char *src, size_t n);
int check_path(char *path, int builtin);
void check_command_name(t_cmd *cmd);
void child_process(t_cmd *cmd, int pipe_nb, int btn, t_mini *box);
void execute_arguments(t_cmd *cmd, t_mini *box);
void sig_wait(t_cmd *cmd);
void execute_pipes(t_cmd *cmd, int pipe_nb, t_mini *box);
// void exec_builtin(int btn, t_cmd *cmd);
// int check_builtin(t_cmd *cmd);
void handle_file_redirections(t_cmd *cmd, int btn);
void execs(t_cmd *cmd, int btn, t_mini *box);
void files_redirections(t_cmd *cmd, int builtin);
void append_file_prep(t_cmd *cmd, char *path, int is_builtin);
void out_file_prep(t_cmd *cmd, char *path, int is_builtin);
void in_file_prep(t_cmd *cmd, char *path, int is_builtin);
int check_file_errors(char *path, int builtin);
void handle_file_redirections(t_cmd *cmd, int btn);
int check_builtin(t_cmd *cmd);



#endif