#include "mini.h"
#include "../minishell.h"

int count_arguments(char **arguments)
{
    int count = 0;
    while (arguments && arguments[count])
    {
        count++;
    }
    return count;
}

void initialize_node(t_ast *node)
{
    node->data->input_fd = STDIN_FILENO;
    node->data->output_fd = STDOUT_FILENO;
}

void create_pipe(t_ast *left_node, t_ast *right_node)
{
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1)
    {
        perror("pipe error");
        exit(EXIT_FAILURE);
    }
    left_node->data->output_fd = pipe_fd[1];  
    right_node->data->input_fd = pipe_fd[0];
}

void initialize_input_redirection(t_ast *cmd, const char *filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("open input error");
        return;
    }
    cmd->data->input_fd = fd;
}

void initialize_output_redirection(t_ast *cmd, const char *filename)
{
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open output error");
        return;
    }
    cmd->data->output_fd = fd;
}

char **get_path(t_mini *box)
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
        arr[i] = ft_strdup(cmd->data->arguments[i]->value); 
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

void execute_ast(t_ast *node, t_mini *box)
{
    if (!node)
        return;
    if (node->type == PIPELINE)
    {
        create_pipe(node->left, node->right);
        pid_t pid_left = fork();
        if (pid_left < 0)
        {
            perror("fork error");
            exit(EXIT_FAILURE);
        }
        if (pid_left == 0)
        {
            execute_ast(node->left, box);
            exit(0);
        }
        pid_t pid_right = fork();
        if (pid_right < 0)
        {
            perror("fork error");
            exit(EXIT_FAILURE);
        }
        if (pid_right == 0) 
        {
            execute_ast(node->right, box);
            exit(0);
        }
        waitpid(pid_left, NULL, 0);
        waitpid(pid_right, NULL, 0); 
        return;
    }
    else if (node->type == REDERECTION_IN)
        initialize_input_redirection(node, node->data->token->value);
    else if (node->type == REDERECTION_OUT)
        initialize_output_redirection(node, node->data->token->value);
    else if (node->type == COMMAND)
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

