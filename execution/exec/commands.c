#include "mini.h"
#include "../../../parsing/minishell.h"

void initialize_node(ASTNode *node)
{
    node->fd_in = STDIN_FILENO;
    node->fd_out = STDOUT_FILENO;
}

void create_pipe(ASTNode *left_node, ASTNode *right_node)
{
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1)
    {
        perror("pipe error");
        return;
    }
    left_node->fd_out = pipe_fd[1];  // Left node writes to the pipe (output)
    right_node->fd_in = pipe_fd[0];  // Right node reads from the pipe (input)
}

void initialize_input_redirection(ASTNode *cmd, const char *filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("open input error");
        return;
    }
    cmd->fd_in = fd;
}

void initialize_output_redirection(ASTNode *cmd, const char *filename)
{
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open output error");
        return;
    }
    cmd->fd_out = fd;
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

char **get_command(ASTNode *cmd)
{
    int i = 0;
    char **arr = malloc(sizeof(char *) * (cmd->arg_nbr + 1));
    if (!arr)
    {
        perror("malloc error");
        return NULL;
    }

    while (i < cmd->arg_nbr)
    {
        arr[i] = ft_strdup(cmd->arguments[i]->value); // Get the command and its arguments
        if (!arr[i])
        {
            perror("ft_strdup error");
            while (--i >= 0)
                free(arr[i]);
            free(arr);
            return NULL;
        }
        i++;
    }
    arr[cmd->arg_nbr] = NULL; // Null-terminate the argument array
    return arr;
}

void execute_ast(ASTNode *node, t_mini *box)
{
    if (!node)
        return;

    if (node->type == PIPELINE)
    {
        create_pipe(node->left, node->right);

        pid_t pid_left = fork();
        if (pid_left == 0) // Child process executes the left command
        {
            execute_ast(node->left, box);
            exit(0);
        }

        pid_t pid_right = fork();
        if (pid_right == 0) // Child process executes the right command
        {
            execute_ast(node->right, box);
            exit(0);
        }

        waitpid(pid_left, NULL, 0); // Wait for the left command to finish
        waitpid(pid_right, NULL, 0); // Wait for the right command to finish
        return;
    }
    else if (node->type == REDERECTION_IN)
    {
        initialize_input_redirection(node, node->token->value);
    }
    else if (node->type == REDERECTION_OUT)
    {
        initialize_output_redirection(node, node->token->value);
    }
    else if (node->type == COMMAND)
    {
        if (is_builtin(node))
        {
            char **av = get_command(node);
            if (!av)
                return;
            int status = builtins(av, box, 0);
            exit(status);
        }
        else // External command
        {
            char **av = get_command(node); 
            if (!av)
                return;

            if (node->fd_in != STDIN_FILENO)
            {
                dup2(node->fd_in, STDIN_FILENO);
                close(node->fd_in);
            }

            if (node->fd_out != STDOUT_FILENO)
            {
                dup2(node->fd_out, STDOUT_FILENO);
                close(node->fd_out);
            }
            char **command_path = get_path(box); 
            if (!command_path)
                return;

            
            int i = 0;
            char *full_path = NULL;
            while (command_path[i])
            {
                full_path = ft_strjoin(command_path[i], "/");
                full_path = ft_strjoin(full_path, av[0]); 
                if (access(full_path, X_OK) == 0) 
                {
                    execve(full_path, av, box->env); 
                }
                i++;
                free(full_path);
            }

            perror("command not found");
            exit(EXIT_FAILURE);
        }
    }
}
