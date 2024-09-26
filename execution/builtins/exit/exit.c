#include "../../../minishell.h"
#include "../../libftt/libft.h"


// void ft_putstr_fd(char *str, int fd)
// {
//     if(!str)
//         return;
//     write(fd, str, strlen(str));
// }

int is_numeric(const char *str)
{
    if (!str || *str == '\0')
        return 0;
    if (*str == '+' || *str == '-')
        str++;
    if (*str == '\0')
        return 0;
    while (*str)
    {
        if (!isdigit(*str))
            return 0;
        str++;
    }
    return 1;
}

int	ft_exit(t_shell *shell)
{
    int exit_status = shell->exit_status;
	ft_putstr_fd("exit\n", 1);
	if (shell->args[1])
		exit_status = atoi(shell->args[1]);
	else
    {
		exit_status = 0;
        return 0;
    }
	if (exit_status < INT_MIN || exit_status > INT_MAX)
		ft_putstr_fd("exit: numeric argument required", 2);
	else if (shell->args[1] && shell->args[2])
	{
		ft_putstr_fd("bash: exit: too many arguments\n", 2);
		return (1);
	}
	else if (shell->args[1] && !is_numeric(shell->args[1]))
		ft_putstr_fd("exit: numeric argument required\n",  2);
	if (!is_numeric(shell->args[1]) || exit_status < INT_MIN || exit_status > INT_MAX)
		exit_status = 2;
	exit(exit_status);
	return (0);
}

// int main(int argc, char *argv[])
// {
//     t_shell shell = {0, argv, 0};  

//     int exit_code;
//     exit_code = ft_exit(&shell);
//          return shell.exit_status;
//     return 0;
// }

