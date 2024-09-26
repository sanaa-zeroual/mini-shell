#include "../../../minishell.h"
#include "../../libftt/libft.h"

int ft_pwd(char **av)
{
    char *pwd;
    t_envi env;

    (void)av;
    (void)env;
    char buff[1024];
    pwd = getcwd(buff, sizeof(buff));
    if(!pwd)
    {
        perror("pwd; failed");
        return 1;
    }
       printf("%s\n", pwd);
        write(1, "\n", 1);
        env.vale= ft_strdup(pwd);
    return 0;
}
