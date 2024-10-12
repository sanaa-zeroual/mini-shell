#include "../minishell.h"

// int	handle_heredoc(char *delim, int flg)
// {
//     t_pipe *pipe_fd;
// 	char	*input;
// 	char	*save;

// 	if (pipe_sc(pipe_fd) < 0)
// 		return (-1);
// 	while (1)
// 	{
// 		input = readline("> ");
// 		if (!input || !ft_memcmp(input, delim, ft_strlen(input) + 1))
// 		{
// 			free(input);
// 			break ;
// 		}
// 		if (flg)
// 			input = expand_in_doc(input);
// 		save = ft_strjoin(input, "\n");
// 		(free(input), input = save);
// 		if (_write_(pipe_fd->write_end, input, ft_strlen(input)) < 0)
// 			return (free(input), -1);
// 		(free(input), input = NULL);
// 	}
// 	return (close(pipe_fd->write_end), (pipe_fd->read_end));
// }

int redir_fd_in(t_ast *cmd) 
{
    t_ast *node = cmd->left;
    int fd = 0;
    while (node)
    {
        if (node->type == REDERECTION_IN)
        {
            if (fd)
                close(fd);
            fd = open(node->data->token->value, O_RDONLY, 00644);
            if (fd < 0)
                return -1;
        }
        else if (node->type == REDERECTION_HEREDOC)
        {
            if (fd)
                close(fd);
            fd = open(node->data->token->value, O_WRONLY | O_CREAT | O_APPEND, 00644);
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
            if (fd)
                close(fd);
            fd = open(node->data->token->value, O_WRONLY | O_CREAT | O_TRUNC, 00644);
            if (fd < 0)
                return -1;
        }
        else if (node->type == REDERECTION_APPEND)
        {
            if (fd)
                close(fd);
            fd = open(node->data->token->value, O_WRONLY | O_CREAT | O_APPEND, 00644);
        }
            if (fd < 0)
                return -1;
        node = node->right;
    }
    cmd->data->output_fd = fd;
    return 0;
}

// int handle_redirections(t_ast *cmd)
// {
//     if((redir_fd_in(cmd) < 0) || (redir_fd_out < 0))
//         return -1;
//     return 0;
// }

// int is_valid(int c)
// {
// 	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
// 		|| (c >= '0' && c <= '9') || c == '_');
// }

// static	ssize_t	calc_ll(char *s)
// {
// 	ssize_t	l;
// 	ssize_t	i;
// 	char	*var;

// 	l = 0;
// 	i = 0;
// 	while (s[i])
// 	{
// 		if (s[i] == '$' && is_valid(s[i + 1]))
// 		{
// 			i++;
// 			var = expand_var(s, &i);
// 			if (!var || !*var)
// 				continue ;
// 			l += ft_strlen(var);
// 		}
// 		else
// 		{
// 			i++;
// 			l++;
// 		}
// 	}
// 	return (l);
// }
 
//  int handle_redirections(t_ast *cmd)
//  {
//     if(redir_fd_in(cmd) < 0 || redir_fd_out(cmd))
//         return -1;
//     return 0;
//  }

// char	*expand_in_doc(char *s)
// {
// 	char	*ret;
// 	char	*save;
// 	char	*var;
// 	ssize_t	i;

// 	ret = malloc(sizeof(char) * (calc_ll(s) + 1));
// 	if (!ret)
// 		printf("hello");
// 	i = 0;
// 	save = ret;
// 	while (s[i])
// 	{
// 		if (s[i] == '$' && is_valid(s[i + 1]))
// 		{
// 			i++;
// 			var = expand_var(s, &i);
// 			if (!var || !*var)
// 				continue ;
// 			while (*var)
// 				*ret++ = *var++;
// 		}
// 		else
// 			*ret++ = s[i++];
// 	}
// 	return (*ret = '\0', free(s), save);
// }

void execute_command(t_ast *cmd)
{
    // if (handle_redirections(cmd) < 0)
    //     return;
    pid_t pid = fork();
    if (pid < 0)
        return;
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
        if (cmd->data->input_fd != 0)
          close(cmd->data->input_fd);
        if (cmd->data->output_fd != 0)
           close(cmd->data->output_fd);
    }
}


