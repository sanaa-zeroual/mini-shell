#include "../../minishell.h"

int builtins(char **av, t_mini *box)
{
    int status = 0;
    if (strcmp(av[0], "cd") == 0)
        status = ft_cd(av, box->env);
    else if (strcmp(av[0], "echo") == 0)
        status = ft_echo(av);
    else if (strcmp(av[0], "pwd") == 0)
        status = ft_pwd(box->env);
    else if (strcmp(av[0], "exit") == 0)
        status = ft_exit(box->shell);
    else if (strcmp(av[0], "export") == 0)
        status = ft_export(av, box->env);
    else if (strcmp(av[0], "env") == 0)
        status = ft_env(box->env);
    else if (strcmp(av[0], "unset") == 0)
        status = ft_unset(av, box);
    return status;
}

int is_builtin(char *cmd)
{
    if (strcmp(cmd, "cd") == 0 || strcmp(cmd, "echo") == 0 ||
        strcmp(cmd, "pwd") == 0 || strcmp(cmd, "exit") == 0 ||
        strcmp(cmd, "export") == 0 || strcmp(cmd, "env") == 0 ||
        strcmp(cmd, "unset") == 0)
    {
        return 1; 
    }
    return 0; 
}