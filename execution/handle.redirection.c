#include "mini.h"
#include "../minishell.h"

int redir_fd_in(t_ast *cmd) 
{
    t_ast *node = cmd->left;
    int fd = 0;
    while (node)
    {
        if (node->type == REDERECTION_IN)
        {
            if (fd) close(fd);
            fd = open(node->data->token->value, O_RDONLY, 00644);
            if (fd < 0) return -1;
        }
        else if (node->type == PREC_REDIR_HERE_DOC)
        {
            if (fd) close(fd);
            fd = node->data->input_fd;
        }
        node = node->right;
    }
    cmd->data->input_fd = fd;
    return 0;
}

int redir_fd_out(t_ast *cmd)
{
    int fd = 0;
    t_ast *node = cmd->left;

       while (node)
       {
        if (node->type == REDERECTION_OUT)
        {
            if (fd) close(fd);
            fd = open(node->data->token->value, O_WRONLY | O_CREAT | O_TRUNC, 00644);
            if (fd < 0) return -1;
        }
        else if (node->type == REDERECTION_APPEND)
        {
            if (fd) close(fd);
            fd = open(node->data->token->value, O_WRONLY | O_CREAT | O_APPEND, 00644);
            if (fd < 0) return -1;
        }
        node = node->right;
    }
    cmd->data->output_fd = fd;
    return 0;
}

void execute_command(t_ast *cmd)
{
    if (redir_fd_in(cmd) < 0) return;
    if (redir_fd_out(cmd) < 0) return;

    pid_t pid = fork();
    if (pid < 0) return;

    if (pid == 0)
    {
        if (cmd->data->input_fd != 0)
        {
            dup2(cmd->data->input_fd, STDIN_FILENO);
            close(cmd->data->input_fd);
        }
        if (cmd->data->output_fd != 0)
        {
            dup2(cmd->data->output_fd, STDOUT_FILENO);
            close(cmd->data->output_fd);
        }
        executing(cmd);
        exit(0);
    } 
    else
    {
        waitpid(pid, NULL, 0);
        if (cmd->data->input_fd != 0) close(cmd->data->input_fd);
        if (cmd->data->output_fd != 0) close(cmd->data->output_fd);
    }
}

