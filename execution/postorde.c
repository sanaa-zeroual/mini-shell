#include "../minishell.h"

void postorder_execution(t_ast *root, t_mini *box)
{
    if(!root)
        return;
    postorder_execution(root->left, box);
    postorder_execution(root->right, box);
    if(root->type == REDERECTION_IN || root->type == REDERECTION_HEREDOC) 
        redir_fd_in(root);
    else if(root->type == REDERECTION_OUT || root->type == REDERECTION_APPEND)
        redir_fd_out(root);
    else if(root->type == PIPELINE)
        execute_pipeline(root, box);
    if(root->type == COMMAND)
        executing(root, box);
}