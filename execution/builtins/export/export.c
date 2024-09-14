#include "../mini.h"
#include "../libftt/libft.h"

void	swap_nodes(t_envi *a, t_envi *b)
{
	char	*tmp_name;
	char	*tmp_vale;

	tmp_name = a->name;
	tmp_vale = a->vale;
	a->name = b->name;
	a->vale = b->vale;
	b->name = tmp_name;
	b->vale = tmp_vale;
}

int	f__plus(char *r)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (r[i])
	{
		if (r[i] == '+')
		{
			count++;
			if (r[i + 1] != '=')
				count++;
		}
		i++;
	}
	if (count > 1)
		return (0);
	return (1);
}

int	ft_utils(char *ptr)
{
	int	i;

	i = 0;
	if (!isalpha(ptr[0]) && ptr[0] != '_')
		return (0);
	while (ptr[++i] && ptr[i] != '=')
	{
		if (ptr[i] == '+')
			break ;
		else if (!isalnum(ptr[i]) && ptr[i] != '_')
			return (0);
	}
	if (ptr[i] && ptr[i] == '+' && ptr[i + 1] != '=')
		return (0);
	return (1);
}

void	add_back(t_envi **node, t_envi *new)
{
	t_envi	*last;

	if (!*node)
	{
		*node = new;
		return ;
	}
	last = *node;
	while (last->next)
		last = last->next;
	last->next = new;
	new->prv = last;
}

t_envi	*add_new_var(char *name, char *vale)
{
	t_envi	*lst;

	lst = malloc(sizeof(t_envi));
	if (!lst)
	{
		perror("allocation error");
		return (NULL);
	}
	lst->name = ft_strdup(name);
	lst->vale = ft_strdup(vale);
	lst->next = NULL;
	lst->prv = NULL;
	return (lst);
}

t_envi	*cpy_list(t_envi *env)
{
	t_envi	*new_list;

	new_list = NULL;
	while (env)
	{
		add_back(&new_list, add_new_var(env->name, env->vale));
		env = env->next;
	}
	return (new_list);
}

void	use_while_env(t_envi *new_env)
{
	int		is_swap;
	t_envi	*curr;

	while (1)
	{
		is_swap = 0;
		curr = new_env;
		while (curr->next != NULL)
		{
			if (ft_strncmp(curr->name, curr->next->name, ft_strlen(curr->name)) > 0)
			{
				swap_nodes(curr, curr->next);
				is_swap = 1;
			}
			curr = curr->next;
		}
		if (!is_swap)
			break ;
	}
}

t_envi	*sort_env(t_envi *env)
{
	t_envi	*new_env;

	new_env = cpy_list(env);
	if (!new_env)
		return (NULL);
	use_while_env(new_env);
	return (new_env);
}

t_envi	*create_env_node(char *name, char *value)
{
	t_envi	*new_node;

	if (!name)
		return (NULL);
	new_node = malloc(sizeof(t_envi));
	if (!new_node)
	{
		perror("error");
		exit(0);
	}
	new_node->name = ft_strdup(name);
	new_node->vale = ft_strdup(value);
	new_node->next = NULL;
	return (new_node);
}

t_envi	*find_last_node(t_envi *env)
{
	t_envi	*curr = env;
	while (curr->next)
		curr = curr->next;
	return (curr);
}

void	add_env_variable(t_envi *env, char *name, char *value)
{
	t_envi	*new_node = create_env_node(name, value);
	t_envi	*last = find_last_node(env);
	last->next = new_node;
}

int	check_plus(char *str, int size_name)
{
	if (str[size_name] == '+' && str[size_name + 1] == '=')
		return (1);
	return (0);
}

void	process_existing_env(t_envi *new, char *ptr_i, char *arr[2])
{
	if (check_plus(ptr_i, ft_strlen(arr[0])))
	{
		char *joined_val = malloc(ft_strlen(new->vale) + ft_strlen(arr[1]) + 1);
		strcpy(joined_val, new->vale);
		strcat(joined_val, arr[1]);
		free(new->vale);
		new->vale = joined_val;
	}
	else if (arr[1])
	{
		free(new->vale);
		new->vale = ft_strdup(arr[1]);
	}
}

t_envi *search_env(t_envi *env, char *name)
{
	while (env)
	{
		if (strcmp(env->name, name) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

int	process_single_env(char *ptr_i, t_envi *env)
{
	char	*arr[2];
	t_envi	*new;
	int		status = 0;

	arr[0] = strtok(ptr_i, "=");
	arr[1] = strtok(NULL, "=");
	if (ft_utils(arr[0]))
	{
		new = search_env(env, arr[0]);
		if (new != NULL)
			process_existing_env(new, ptr_i, arr);
		else
			add_env_variable(env, arr[0], arr[1]);
	}
	else
	{
		printf("export: `%s': not a valid identifier\n", ptr_i);
		status = 1;
	}
	return (status);
}

int	add_one(char **ptr, t_envi *env)
{
	int	i = 1;
	int	status = 0;
	int	result;

	while (ptr[i])
	{
		result = process_single_env(ptr[i], env);
		if (result)
			status = result;
		i++;
	}
	return (status);
}

int	ft_export(char **ptr, t_envi *env)
{
	t_envi	*newenv;
	int		status = 0;

	status = add_one(ptr, env);
	if (!ptr[1])
	{
		newenv = sort_env(env);
		while (newenv)
		{
			printf("declare -x %s", newenv->name);
			if (newenv->vale)
				printf("=\"%s\"", newenv->vale);
			printf("\n");
			newenv = newenv->next;
		}
	}
	return (status);
}

