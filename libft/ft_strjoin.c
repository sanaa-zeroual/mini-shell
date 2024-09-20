#include "../minishell.h"

static char	*check_str(char const *s1, char const *s2)
{
	if (!s1 && s2)
		return (strdup(s2));
	if (s1 && !s2)
		return (strdup(s1));
	return (NULL);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		len;
	int		i;
	char	*new;
	int		j;

	i = 0;
	j = 0;
	if (s1 == NULL || s2 == NULL)
		return (check_str(s1, s2));
	len = strlen(s1) + strlen(s2);
	new = (char *) malloc(len + 1);
	if (!new)
		return (NULL);
	while (s1[i])
	{
		new[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		new[i++] = s2[j++];
	}
	new[i] = '\0';
	return (new);
}