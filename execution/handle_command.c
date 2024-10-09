#include "../minishell.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char **separate_env(t_envi *env)
{
    int count = 0;
    t_envi *tmp = env;
    while (tmp)
    {
        count++;
        tmp = tmp->next;
    }
    char **the_env = malloc(sizeof(char *) * (count + 1));
    if (!the_env)
    {
        perror("Error allocating memory");
        return NULL;
    }
    int i = 0;
    while (env)
    {
        size_t name_len = strlen(env->name);
        size_t value_len = strlen(env->vale);
        the_env[i] = malloc(name_len + value_len + 2);
        if (!the_env[i])
        {
            perror("Error allocating memory for environment variable string");
            return NULL;
        }
        strcpy(the_env[i], env->name);
        the_env[i][name_len] = '=';
        strcpy(the_env[i] + name_len + 1, env->vale);
        env = env->next;
        i++;
    }
    the_env[i] = NULL;
    return the_env;
}

int count_arguments(char **arguments)
{
    int count = 0;
    while (arguments && arguments[count])
    {
        count++;
    }
    return count;
}

char **get_path()
{
    char *path_env = getenv("PATH");
    if (!path_env)
    {
        perror("PATH not found");
        return NULL;
    }
    return ft_split(path_env, ':');
}

char **get_command(t_ast *cmd)
{
    int arg_count = count_arguments(cmd->data->arguments);
    char **arr = malloc(sizeof(char *) * (arg_count + 1));
    if (!arr)
    {
        perror("malloc error");
        return NULL;
    }
    int i = 0;
    while (i < arg_count)
    {
        arr[i] = ft_strdup(cmd->data->arguments[i]);
        if (!arr[i])
        {
            perror("ft_strdup error");
            free(arr);
            return NULL;
        }
        i++;
    }
    arr[arg_count] = NULL;
    return arr;
}

void executing(t_ast *node)
{
    t_mini *box = NULL;
    if (node->type == COMMAND)
    {
        char **av = get_command(node);
        if (!av)
            return;
        if (!is_builtin(node->data->token->value))
        {
            char **command_path = get_path();
            if (!command_path)
            {
                free(av);
                return;
            }
            char **env_array = separate_env(box->env);
            if (!env_array)
            {
                free(av);
                free(command_path);
                return;
            }
            if (node->data->input_fd != STDIN_FILENO)
            {
                dup2(node->data->input_fd, STDIN_FILENO);
                close(node->data->input_fd);
            }
            if (node->data->output_fd != STDOUT_FILENO)
            {
                dup2(node->data->output_fd, STDOUT_FILENO);
                close(node->data->output_fd);
            }
            int i = 0;
            char *full_path = NULL;
            while (command_path[i])
            {
                char *temp = ft_strjoin(command_path[i], "/");
                full_path = ft_strjoin(temp, av[0]);
                free(temp);
                if (access(full_path, X_OK) == 0) 
                {
                    execve(full_path, av, env_array);
                    perror("execve error");
                    exit(EXIT_FAILURE); 
                }
                free(full_path);
                i++;
            }
            perror("command not found");
            free(av);
            i = 0;
            while (command_path[i])
            {
                free(command_path[i]);
                i++;
            }
            free(command_path);
            i = 0;
            while (env_array[i])
            {
                free(env_array[i]);
                i++;
            }
            free(env_array);
            exit(EXIT_FAILURE);
        }
        else
        {
            int status = builtins(av, box);
            free(av);
            exit(status);
        }
    }
}


