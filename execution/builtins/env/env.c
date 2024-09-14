#include "../mini.h"
#include "../libftt/libft.h"


int *ft_env(t_envi *env)
{
    t_envi *tmp = env;
    while(tmp)
    {
        if(tmp->vale)
        {
            ft_putstr_fd(tmp->name, 1);
            ft_putstr_fd("=", 1);
            ft_putstr_fd(tmp->vale, 1);
        }
        tmp= tmp->next;
    }
    return 0;
}


void ft_remove(t_mini *box)
{
    char **av =box->ptr;
    t_envi *curr = box->env;
    t_envi *prev;
    int i = 0;

    while(av[i])
    {
        curr = box->env;
        prev = NULL;
        while(curr && strcmp(curr->name, av[i]) == 0)
        {
            if(prev)
                prev->next = curr->next;
            else
                box->env = curr->next;
        }
        if(curr && strcmp(curr->name, "_") == 0)
       {
        prev = curr;
        curr = curr->next;
       }
    free(curr->name);
    free(curr->vale);
    free(curr);
    }
    i++;
}



int	ft_unset(char **ptr, t_mini *box)
{
	t_envi	*current;
	t_envi	*previous;

	current = NULL;
	previous = NULL;
	if (ptr[1])
	{
		use_while(ptr, current, previous, box);
	}
	else
		return (0);
	return (0);
}
