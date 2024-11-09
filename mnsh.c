/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mnsh.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:53:32 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/07 15:55:20 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_var g_var;

t_shell *init_shell()
{
    t_shell *shell = ft_malloc(sizeof(t_shell), 1);
    if (!shell)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    shell->exit_status = 0;
    shell->args = NULL;
    return shell;
}
t_envi *init_env(char **envp)
{
    t_envi *env_list = NULL;
    t_envi *new_node;
    char *name;
    char *value;
    int i = 0;

    while (envp[i])
    {
        char *env_entry = strdup(envp[i]);
        if (!env_entry)
        {
            perror("strdup");
            exit(EXIT_FAILURE);
        }

        name = strtok(env_entry, "=");
        value = strtok(NULL, "=");
        if (!name || !value)
        {
            fprintf(stderr, "Invalid environment variable format: %s\n", envp[i]);
            free(env_entry);
            i++;
            continue;
        }
        new_node = ft_malloc(sizeof(t_envi), 1);
        if (!new_node)
        {
            perror("malloc");
            free(env_entry);
            exit(EXIT_FAILURE);
        }
        new_node->name = strdup(name);
        new_node->vale = strdup(value);
        if (!new_node->name || !new_node->vale)
        {
            perror("strdup");
            free(new_node->name);
            free(new_node->vale);
            free(new_node);
            free(env_entry);
            exit(EXIT_FAILURE);
        }
        new_node->next = env_list;
        new_node->prv = NULL;
        if (env_list)
            env_list->prv = new_node;
        env_list = new_node;
        free(env_entry);
        i++;
    }
    return env_list;
}
void	print_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd)
	{
		printf("=======Arguments=======\n");
		if (cmd->arguments)
		{
			i = 0;
			while (cmd->arguments[i])
			{
				printf("arg[%d] == %s\n", i, cmd->arguments[i]);
				i++;
			}
		}
		if (cmd->file)
		{
			while (cmd->file)
			{
				printf("filename == %s\n", cmd->file->filename);
				cmd->file = cmd->file->next;
			}
		}
		cmd = cmd->next;
	}
}

int main(int argc, char **argv, char **envp)
{
    char *input;
    t_token **tokens;
    t_cmd *cmd;
    t_mini *box;
    (void)argc;
    (void)argv;

    tokens = NULL;
    box = ft_malloc(sizeof(t_mini), 1);
    if (!box)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    box->env = init_env(envp);
    box->shell = init_shell();
    box->ptr = NULL;
    box->arr = NULL;
    box->last_exit_status = 0;
    while (1)
    {
        handle_signal();
		input = readline("minishell > ");
		if (!input)
			break ;
		if (!ft_strlen(input))
			continue ;
		add_history(input);
		tokens = tokenize(input);
		if (check_syntax_errors(*tokens))
			continue ;
		if (!expand(*tokens))
			continue;
		cmd = analyse_tokens(tokens);
        execute_arguments(cmd, box);
        free(input);
        // clean_gc();
    }
    return 0;
}
