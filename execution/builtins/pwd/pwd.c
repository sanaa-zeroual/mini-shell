#include "../../../minishell.h"
#include "../../libftt/libft.h"

// int ft_pwd(char **av)
// {
//     char *pwd;
//     t_envi env;

//     (void)av;
//     (void)env;
//     char buff[1024];
//     pwd = getcwd(buff, sizeof(buff));
//     if(!pwd)
//     {
//         perror("pwd; failed");
//         return 1;
//     }
//        printf("%s\n", pwd);
//         write(1, "\n", 1);
//         env.vale= ft_strdup(pwd);
//     return 0;
// }

int ft_pwd(t_envi *env)
{
    char buff[1024];
    char *pwd = getcwd(buff, sizeof(buff));
    if (!pwd)
    {
        perror("pwd; failed");
        return 1;
    }
    printf("%s\n", pwd);
    write(1, "\n", 1);
    if (env) {
        free(env->vale);
        env->vale = ft_strdup(pwd);
        if (!env->vale) {
            perror("Failed to duplicate pwd");
            return 1;
        }
    }
    return 0;
}