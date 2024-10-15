#include "../minishell.h"

void executing(t_ast *node, t_mini *box)
{
    if (node->type == COMMAND)
    {
        t_parser *av = node->data;
        if (!av->arguments)
            return;
        int j = 0;
        while(av->arguments[j])
        {
            printf("%s", av->arguments[j]);
            j++;
        }
        if(is_builtin(node->data->token->value))
        {
            int status = builtins(av->arguments, box);
            exit(status);
        }
        else
        {
            char **command_path = get_path();
            if (!command_path)
            {
                free(av->arguments);
                return;
            }
            char **env_array = separate_env(box->env);
            if (!env_array)
            {
                free(av->arguments);
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
                if(!temp)
                    return ;
                full_path = ft_strjoin(temp, av->arguments[0]);
                free(temp);
                if (access(full_path, X_OK) == 0) 
                {
                    execve(full_path, av->arguments, env_array);
                    perror("execve error");
                    exit(EXIT_FAILURE); 
                }
                free(full_path);
                i++;
            }
            perror("command not found");
            free(av->arguments);
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
    }
}
