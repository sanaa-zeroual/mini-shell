#include "../minishell.h"

void executing(t_ast *node, t_mini *box)
{
    if (node->type == COMMAND)
    {
        if (!node->data->arguments)
            return;

        if (is_builtin(node->data->token->value))
        {
            builtins(node->data->arguments, box); // Execute built-in command
            return; // Return to allow shell to continue running
        }
        else
        {
            pid_t pid = fork(); // Create a child process

            if (pid < 0) {
                perror("Fork failed");
                return;
            }

            if (pid == 0) // Child process
            {
                char **command_path = get_path();
                if (!command_path) {
                    free(node->data->arguments);
                    exit(EXIT_FAILURE);
                }
                char **env_array = separate_env(box->env);
                if (!env_array) {
                    free(node->data->arguments);
                    free(command_path);
                    exit(EXIT_FAILURE);
                }

                // Handle input and output redirection
                if (node->data->input_fd != STDIN_FILENO) {
                    dup2(node->data->input_fd, STDIN_FILENO);
                    close(node->data->input_fd);
                }
                if (node->data->output_fd != STDOUT_FILENO) {
                    dup2(node->data->output_fd, STDOUT_FILENO);
                    close(node->data->output_fd);
                }

                // Attempt to execute the command
                int i = 0;
                char *full_path = NULL;
                while (command_path[i]) {
                    char *temp = ft_strjoin(command_path[i], "/");
                    if (!temp) {
                        free(node->data->arguments);
                        exit(EXIT_FAILURE);
                    }
                    full_path = ft_strjoin(temp, node->data->arguments[0]);
                    free(temp);
                    if (access(full_path, X_OK) == 0) 
                    {
                        execve(full_path, node->data->arguments, env_array);
                        perror("execve error"); // If execve fails
                        exit(EXIT_FAILURE); 
                    }
                    free(full_path);
                    i++;
                }

                perror("command not found");
                free(node->data->arguments);
                i = 0;
                while (command_path[i]) {
                    free(command_path[i]);
                    i++;
                }
                free(command_path);
                i = 0;
                while (env_array[i]) {
                    free(env_array[i]);
                    i++;
                }
                free(env_array);
                exit(EXIT_FAILURE); // Exit child process on error
            }
            else // Parent process
            {
                int status;
                waitpid(pid, &status, 0); // Wait for child to finish
                // Optionally handle status here
            }
        }
    }
}
