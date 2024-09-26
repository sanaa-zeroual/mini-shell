#include "../../../minishell.h"
#include "../../libftt/libft.h"

// t_envi	*search_env(t_envi *envi, char *name)
// {
// 	while (envi)
// 	{
// 		if (strcmp(envi->name, name)==0)
// 			return (envi);
// 		envi = envi->next;
// 	}
// 	return (NULL);
// }

void	update_env(t_envi *envi)
{
	t_envi	*pwd;
	t_envi	*old_pwd;
	char	tmp[1024];

	pwd = search_env(envi, "PWD");
	old_pwd = search_env(envi, "OLDPWD");
	if (old_pwd)
		old_pwd->vale = pwd->vale;
	if (pwd)
	{
		getcwd(tmp, 1024);
		pwd->vale = ft_strdup(tmp);
	}
}

int	ft_cd(char **ptr, t_envi *envi)
{
	if (ptr[0] && !ptr[1]) 
	{
		ptr[1] = (search_env(envi, "HOME"))->vale;
		if (ptr[1] == NULL)
		{
			printf("cd: HOME not set\n");
			return (1);
		}
	}
	if (chdir(ptr[1]) != 0)
	{
		perror("cd failed");
		return (1);
	}
	update_env(envi);
	return (0);
}
