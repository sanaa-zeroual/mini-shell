#include"mini.h"
#include "../minishell.h"

pid_t left_pipe(t_ast *cmd, t_pipe *pipe_fds)
{
    pid_t pipe_left = fork();
    if (pipe_left < 0)
    {
        perror("error pipe");
        return -1;
    }
    if (pipe_left == 0)
    {
        if (dup2(pipe_fds->write_end, STDOUT_FILENO) < 0)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(pipe_fds->read_end);
        close(pipe_fds->write_end);
        executing(cmd);
        exit(0);
    }
    return pipe_left; 
}

pid_t right_pipe(t_ast *cmd, t_pipe *pipe_fds)
{
    pid_t pipe_right = fork();
    if (pipe_right < 0)
    {
        perror("error");
        return -1;
    }
    if (pipe_right == 0)
    {
        if (dup2(pipe_fds->read_end, STDIN_FILENO) < 0)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(pipe_fds->write_end);
        close(pipe_fds->read_end);
        executing(cmd);
        exit(0);
    }
    return pipe_right;
}

void execute_pipeline(t_ast *cmd, t_pipe *pipe_fds)
{
    pid_t left_pid = left_pipe(cmd->left, pipe_fds);
    pid_t right_pid = right_pipe(cmd->right, pipe_fds);
    close(pipe_fds->read_end);
    close(pipe_fds->write_end);
    waitpid(left_pid, NULL, 0);
    waitpid(right_pid, NULL, 0);
    
}
