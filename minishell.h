/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:39:23 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/19 10:00:21 by shebaz           ###   ########.fr       */
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

typedef struct s_alst
{
	void *content;
	struct s_alst *next;
}t_alst;

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
	int is_herdoc_end;
    struct s_cmd *prev;
    struct s_cmd *next;
    char *cmd_path; // The path of the command
    int pipe_fd[2];
    int pid;
} t_cmd;

typedef struct s_var
{
	t_alst *alist;
    int exit_status; // The exit status
    int out_fd;
	int in_fd;
    int red_error; //error  for redir
    int pre_pipe_infd; // si il ya une commande avant le pipe
    int last_child_id;
    char **envp;
	int	pipe_nb; // the number of pipes 
    int size; // the size of the command
} t_var;


typedef struct garbage_collector
{
	void						*ptr;
	struct garbage_collector	*next;
}								t_gc;

typedef struct global
{
	int							exit_status;
	int							pre_pipe_infd;
	int							last_child_id;
	int							in_fd;
	int							out_fd;
	int							red_error;
	t_envi						*envp;
	int							size;
	int							pipe_nb;
	int							fd_here_doc;
	t_gc						*head;

}								t_globalvar;

extern t_globalvar				*g_var;

t_token							**tokenize(char *input);
char							*handle_quote(char *str);
int								is_charactere(char c);
void							case_function(char *input, char **result,
									int *j);
int								is_special(char c);
int								check_quote(char *str);
int								is_number(char c);
int								one_dollar_test_case(int dollar_count,
									char *input, int *i);
int								dollar_counter(char *input);
void							exit_status_case(char *input, char **result,
									int *i, int *flag);
char							*single_quote_expansion(char *input, int *i);
char							*process_word(char *word);
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
void							fill_up_node(t_cmd **node, t_token **tokens,
									t_file **head);
char							*dollar_expand(char *input, int *i);
void							go_to_next(t_token **tokens);
char							**result_traitement(char *input);
char							*get_string(char *input, int *i);
int								get_size(char **arr);
int								get_size_arr(char *input);
int								file_expansion_null(t_token *tokens);
char							*parse_line(char *input);
char							**handle_that_shit(char *input);
char							**unquoted_result(char **input);
char							*get_word_to_expand(char *str, int *j,
									char **result);
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
char							*ft_getenv(char *word);
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
t_envi							*init_env(char **envp);
///////////////////// execution /////////////////////////

/////////////////////////////builtins///////////////////////////
int								is_builtin(char *cmd);
void							ft_putstr_fd(char *str, int fd);
int								ft_cd(char **ptr, t_envi *envi);
void							update_env(t_envi *envi);
t_envi							*search_env(t_envi *envi, char *name);
int								is_n_option(char *arg);
int								first_non_option(char **args);
int								ft_echo(char **args);
int								ft_unset(char **ptr, t_mini *box);
int								ft_pwd(char **args, t_envi *env);
int								ft_exit(char **args);
int								ft_env(t_envi *env);
void							add_env_variable(t_envi **env, char *name,
									char *value);
t_envi							*sort_env(t_envi *env);
int								ft_utils(char *ptr);
void							swap_nodes(t_envi *a, t_envi *b);
t_envi							*cpy_list(t_envi *env);
int								check_each_element(char *str);
void							ft_remove(char **ptr, t_mini *box, int i);

int								ft_cd(char **ptr, t_envi *envi);
void							update_env(t_envi *envi);
t_envi							*search_env(t_envi *envi, char *name);
int								ft_echo(char **args);
int								first_non_option(char **args);
int								is_n_option(char *arg);
int								ft_export(char **ptr, t_envi **env);
int								ft_unset(char **ptr, t_mini *box);
int								ft_env(t_envi *env);


// extenal command

char				**separate_env(t_envi *env);
char				**get_path(void);
int					count_arguments(char **arguments);

void execute_pipe(t_cmd *cmd, t_mini *box);


////////////////////////////////new shell////////////////////////////////
void validate_cmd(t_cmd *cmd);
char *allocate_folders(char *path, int i);
void check_cmd_path(t_cmd *cmd);
void my_strncpy(char *dest, const char *src, size_t n);
int check_path(char *path, int builtin);
void check_command_name(t_cmd *cmd);
void child_process(t_cmd *cmd, int pipe_nb, int btn, t_mini *box);
void execute_arguments(t_cmd *cmd, t_mini *box);
void sig_wait(t_cmd *cmd);
void execute_pipes(t_cmd *cmd, int pipe_nb, t_mini *box);
void exec_builtin(int btn, t_cmd *cmd, t_mini *box);
void handle_file_redirections(t_cmd *cmd, int btn);
void execs(t_cmd *cmd, int btn, t_mini *box);
void files_redirections(t_cmd *cmd, int builtin);
void append_file_prep(char *path);
void out_file_prep(char *path, int builtin);
void in_file_prep(char *path, int builtin);
int check_file_errors(char *path, int builtin);
// void handle_file_redirections(t_cmd *cmd, int btn);
int check_builtin(t_cmd *cmd);
int count_commands(t_cmd *cmd);
void error_pipe();
void close_files(t_cmd *token);
t_envi *create_env_node(char *name, char *value);
t_envi *create__node(char *name, char *value);
void add_env_node(t_envi **env_list, t_envi *new_node);
void	initiale_global(t_envi *env);
void search_command_in_paths(t_cmd *cmd, char **path_dirs);
void handle_command_not_found(t_cmd *cmd);
void free_path_dirs(char **path_dirs);
int is_executable(char *path);
char *construct_full_path(char *dir, char *cmd);
void error_strdup();
void initialisation(t_mini *box, char **envp);
void handle_input(char *input, t_mini *box);
void shell_loop(t_mini *box);
void process_env_entry(char *env_entry, t_envi **env_list);
t_shell *init_shell();
#endif