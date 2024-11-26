/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 19:00:17 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/11 22:45:55 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*process_word(char *word)
{
	char	*result;
	int		i;

	i = 0;
	result = ft_strdup("");
	while (word[i])
	{
		if (word[i] == '$' && word[i + 1] == '?')
		{
			result = ft_strjoin(result, ft_itoa(g_var->exit_status));
			i++;
		}
		else
			result = ft_strjoin(result, char_to_string(word[i], 0));
		i++;
	}
	return (result);
}

void	exit_status_case(char *input, char **result, int *i, int *flag)
{
	if (input[*i] == '$' && input[*i + 1] == '?')
	{
		(*result) = ft_itoa(g_var->exit_status);
		(*i) += 2;
		*flag = 1;
	}
}

int	one_dollar_test_case(int dollar_count, char *input, int *i)
{
	if (dollar_count == 1 && ft_strlen(input) == 1)
	{
		(*i) += ft_strlen(input);
		return (1);
	}
	return (0);
}

void	case_function(char *input, char **result, int *j)
{
	char	*tmp;

	tmp = ft_strndup(input + *j - 1, *j);
	if (!strcmp(tmp, "$\""))
		*result = ft_strdup("$");
}

int	is_charactere(char c)
{
	if (c == ' ' || c == '\t' || (c >= 'a' && c <= 'z') || (c >= 'A'
			&& c <= 'Z'))
		return (1);
	return (0);
}
