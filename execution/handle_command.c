#include"mini.h"
#include "../minishell.h"

void executing(t_ast *node, t_mini *box)
{
    if(node->type == COMMAND)
    {
        char **av = get_command(node);
        if (!av)
            return;
        if (is_builtin(node)) 
        {
            int status = builtins(av, box, 0);
            free(av);
            exit(status);
        }
        else
        {
            char **command_path = get_path(box);
            if (!command_path)
            {
                free(av);
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
                    execve(full_path, av, box->env);
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
            exit(EXIT_FAILURE);
        }
    }
}