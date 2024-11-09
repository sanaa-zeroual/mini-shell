#include "../../../minishell.h"

int ft_env(t_envi *env)
{
    t_envi *tmp = env;
    while(tmp)
    {
        if(tmp->vale)
        {
            ft_putstr_fd(tmp->name, 1);
            ft_putstr_fd("=", 1);
            ft_putstr_fd(tmp->vale, 1);
            ft_putstr_fd("\n", 1);
        }
        tmp= tmp->next;
    }
    return 0;
}

