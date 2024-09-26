#include "../minishell.h"


void executing_(t_ast *cmd, t_pipe *pipefds)
{
    if(!cmd)
        return;
    if(cmd->type == PIPELINE)
        execute_pipeline(cmd, pipefds);
    else if(cmd->type == REDERECTION_IN || cmd->type == REDERECTION_HEREDOC)
    {
            redir_fd_in(cmd);
            execute_command(cmd);
    }
    else if(cmd->type == REDERECTION_OUT || cmd->type == REDERECTION_APPEND)
     {
        redir_fd_out(cmd);
        execute_command(cmd);
     } 
    else if(cmd->type == COMMAND)
    {
        char **av = get_command(cmd);
        if(is_builtin(cmd->data->token->value))
        {
            builtins(av, NULL);
            free(av);
            exit(0);
        }
        else
        {
            char *path = get_path();

        }
    }
}
