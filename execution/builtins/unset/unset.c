#include "../mini.h"
#include "../libftt/libft.h"

void ft_remove(t_mini *box)
{
	t_envi *curr;
	t_envi *prv;
	char **av = box->ptr;
	int i = 0;

	while(av[i])
	{
		curr= box->env;
		prv = NULL;
		while(curr && ft_strcmp(curr->name, av[i]) != 0)
		{
			prv = curr;
			curr = curr->next;
		}
		if(curr && ft_strcmp(curr->name, "_") != 0)
		{
			if(prv)
				prv->next = curr->next;
			else
				box->env = curr->next;
			free(curr->name);
			free(curr->vale);
			free(curr);
		}
		i++;
	}
}

int	ft_unset(char **ptr, t_mini *box)
{
	t_envi	*current;
	t_envi	*previous;

	current = box->env = NULL;
	previous = NULL;
	if (ptr[1])
		ft_remove(box);
	else
		return (0);
	return (0);
}