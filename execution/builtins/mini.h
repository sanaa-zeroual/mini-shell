#ifndef MINI_H
# define MINI_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
 #include <limits.h>
#include <stdlib.h>
#include<stddef.h>
#include <string.h>
#include<stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include<stddef.h>
#include<stdlib.h>

typedef struct s_env
{
	char					*name;
	char					*vale;
	struct s_env			*next;
    struct s_env             *prv;
}							t_envi;

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
    int in_child;
} t_shell;

typedef struct s_mini
{
    t_envi *env;
    t_shell *shell;
    char **ptr;
    char **arr;
}t_mini;

int builtins(char **av, t_mini *box, int val);
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
int  ft_pwd(char **av);
int	ft_exit(t_shell *shell);
int *ft_env(t_envi *env);
#endif