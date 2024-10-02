#include "../minishell.h"

void process_node(t_ast *cmd, t_pipe *pipe_fds)
{
    if(cmd->type == PIPELINE)
    {
        redir_fd_in(cmd);
        execute_pipeline(cmd);
    }
    else if(cmd->type == REDERECTION_IN || cmd->type == REDERECTION_OUT)
        execute_command(cmd);
    else if(cmd->type == COMMAND)
        executing(cmd);
}

void dfs_postorder(t_ast *cmd)
{
    t_pipe *pipe_fds;
    dfs_postorder(cmd->left);
    dfs_postorder(cmd->right);
    process_node(cmd, pipe_fds);
}
