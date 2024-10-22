#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../minishell.h"

int rfd_in(t_ast *cmd)
{
    t_ast *n = cmd->left;
    int fd = -1;
    while (n)
    {
        if (n->type == REDERECTION_IN)
        {
            if (fd > 0) close(fd);
            fd = open(n->data->arguments[1], O_RDONLY);
            if (fd < 0) {
                perror("Error opening input file");
                return -1;
            }
        }
        n = n->right;
    }
    cmd->data->input_fd = fd;
    return fd;  
}

int rfd_out(t_ast *cmd)
{
    t_ast *n = cmd->left;
    int fd = -1;
    while (n)
    {
        if (n->type == REDERECTION_OUT)
        {
            if (fd > 0) close(fd);
            fd = open(n->data->arguments[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("Error opening output file");
                return -1;
            }
        }
        n = n->right;
    }
    cmd->data->output_fd = fd;
    return fd;  
}

void exec_command_with_redirection(t_ast *cmd)
{
    int in_fd = rfd_in(cmd);
    int out_fd = rfd_out(cmd);
    pid_t pid = fork(); 
    if (pid < 0) {
        perror("Fork failed");
        return;
    }
    if (pid == 0)
    {  
        if (in_fd >= 0)
        {
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
        }
        if (out_fd >= 0)
        {
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
        }
        execvp(cmd->data->arguments[0], cmd->data->arguments);
        perror("Execution failed");
        _exit(1); 
    } 
    else 
    {
        waitpid(pid, NULL, 0);
        if (in_fd >= 0) close(in_fd);
        if (out_fd >= 0) close(out_fd);
    }
}

