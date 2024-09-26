#include"mini.h"
#include "../../parsing/minishell.h"


// void handle_command(t_ast *cmd, t_mini *box)
// {
//     char **argv = get_command(cmd);

//     if (!argv)
//         return;

//     if (is_builtin(cmd))
//     {
//         int status = builtins(argv, box, 0);
//         free(argv);
//         exit(status);
//     }
//     else
//     {
//         char **path = get_path(box);
//         if (!path)
//         {
//             free(argv);
//             return;
//         }
        
//         if (cmd->fd_in != STDIN_FILENO)
//         {
//             dup2(cmd->fd_in, STDIN_FILENO);
//             close(cmd->fd_in);
//         }
//         if (cmd->fd_out != STDOUT_FILENO)
//         {
//             dup2(cmd->fd_out, STDOUT_FILENO);
//             close(cmd->fd_out);
//         }
//         execute_with_path(argv, path, box);
//     }
// }