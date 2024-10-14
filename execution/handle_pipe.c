#include "../minishell.h"

pid_t left_pipe(t_ast *cmd, t_pipe *pipe_fds, t_mini *box)
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
        executing(cmd, box);
        exit(0);
    }
    return pipe_left; 
}

pid_t right_pipe(t_ast *cmd, t_pipe *pipe_fds, t_mini *box)
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
        executing(cmd, box);
        exit(0);
    }
    return pipe_right;
}
   
int create_pipe(t_pipe *pipe_fds)
{
    int fds[2];
    if (pipe(fds) < 0)
    {
        perror("pipe error");
        return -1;
    }
    pipe_fds->read_end = fds[0];
    pipe_fds->write_end = fds[1];
    return 0;
}


void execute_pipeline(t_ast *cmd, t_mini *box)
{
    t_pipe *pipe_fds = malloc(sizeof(t_pipe));
    if(create_pipe(pipe_fds) < 0)
        return;
    pid_t left_pid = left_pipe(cmd->left, pipe_fds, box);
    pid_t right_pid = right_pipe(cmd->right, pipe_fds, box);
    close(pipe_fds->read_end);
    close(pipe_fds->write_end);
    waitpid(left_pid, NULL, 0);
    waitpid(right_pid, NULL, 0);
    
}
