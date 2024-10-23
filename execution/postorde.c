#include "../minishell.h"


void algo_execution(t_ast *cmd, t_mini *box)
{
    if (!cmd)
        return;
    if (cmd->type == PIPELINE)
    {
        execute_pipeline(cmd, box);
    }
    else if (cmd->type == REDERECTION_OUT || cmd->type == REDERECTION_IN)
    {
        exec_command_with_redirection(cmd);
    }
    else if (cmd->type == COMMAND)
        executing(cmd,box);
    algo_execution(cmd->left, box);
    algo_execution(cmd->right, box);
}
// void algo_execution(t_ast *cmd, t_mini *box)
// {
//     if (!cmd)
//         return;
//     // printf("cmd->value = %s\n", cmd->data->token->value);
//     // exit(1);
//     // printf("cmd->data->token = %s", cmd->data->token->expanded_value);
//     // exit(1);
//     if (cmd->type == COMMAND)
//     {
//         // printf("check\n");
//         // exit(1);
//         if (rfd_in(cmd) < 0)
//             return;
//         if (rfd_out(cmd) < 0)
//             return;
//         executing(cmd, box);
//     }
//     else if (cmd->type == PIPELINE)
//     {
//         execute_pipeline(cmd, box);
//         printf("pipe");
//         // exit(1);
//     }
//     else if (cmd->type == REDERECTION_OUT)
//     {
//         // printf("heeey there\n");
//         // // exit(1);
//         // rfd_out(cmd);
//         int saved_stdou = dup(STDOUT_FILENO);
//         if(dup2(cmd->data->output_fd, STDOUT_FILENO) < 0)
//         {
//             printf("ERROR");
//             return ;
//         }
//         algo_execution(cmd->left, box);
//         dup2(saved_stdou, STDOUT_FILENO);
//         close(saved_stdou);
//     }
//     // else if(cmd->type == REDERECTION_IN)
//     // {
//     //     // printf("hy in");
//     //     // exit(1);
//     //     // rfd_in(cmd);
//     //     algo_execution(cmd->left, box);
//     //     // exec_cmd(cmd, box);
//     // }
//     algo_execution(cmd->right, box);
//     algo_execution(cmd->left, box);
// }

// // void algo_execution(t_ast *cmd, t_mini *box)
// // {
// //     if (!cmd)
// //         return;

// //     if (cmd->type == COMMAND) {
// //         // Handle input redirection
// //         if (rfd_in(cmd) < 0)
// //             return;
// //         if (cmd->data->input_fd > 0) {
// //             dup2(cmd->data->input_fd, STDIN_FILENO);
// //             close(cmd->data->input_fd);
// //         }

// //         // Handle output redirection
// //         if (rfd_out(cmd) < 0)
// //             return;
// //         if (cmd->data->output_fd > 0) {
// //             dup2(cmd->data->output_fd, STDOUT_FILENO);
// //             close(cmd->data->output_fd);
// //         }

// //         // Execute the command
// //         executing(cmd, box);
// //     }
// //     else if (cmd->type == PIPELINE) {
// //         execute_pipeline(cmd, box);
// //     }
// //     else if (cmd->type == REDERECTION_OUT) {
// //         int saved_stdout = dup(STDOUT_FILENO);
// //         if (dup2(cmd->data->output_fd, STDOUT_FILENO) < 0) {
// //             perror("dup2 error");
// //             return;
// //         }
// //         algo_execution(cmd->left, box);
// //         dup2(saved_stdout, STDOUT_FILENO);
// //         close(saved_stdout);
// //     }
// //     else if (cmd->type == REDERECTION_IN) {
// //         int saved_stdin = dup(STDIN_FILENO);
// //         if (dup2(cmd->data->input_fd, STDIN_FILENO) < 0) {
// //             perror("dup2 error");
// //             return;
// //         }
// //         algo_execution(cmd->left, box);
// //         dup2(saved_stdin, STDIN_FILENO);
// //         close(saved_stdin);
// //     }

// //     // Recursively execute the left and right children of the AST
// //     algo_execution(cmd->left, box);
// //     algo_execution(cmd->right, box);
// // }
