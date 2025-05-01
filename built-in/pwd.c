#include "minishell.h"
#include <unistd.h>

int	ft_pwd(void)
{
	char	pwd[1024];

	if (getcwd(pwd, sizeof(pwd)) != NULL)
	{
		ft_putendl_fd(pwd, STDOUT_FILENO);
		return (0);
	}
	else
	{
		ft_putstr_fd("minishell: pwd: ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		return (1);
	}
}

void	ft_pwd_no_nl(void)
{
	char	pwd[1024];

	if (getcwd(pwd, sizeof(pwd)) != NULL)
		ft_putstr_fd(pwd, STDOUT_FILENO);
	else
	{
		ft_putstr_fd("minishell: pwd: ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
	}
}
