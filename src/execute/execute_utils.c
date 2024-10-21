#include "../../includes/minishell.h"

// Function to check if cd has too many arguments
int	many_args(int num_args)
{
	if (num_args > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	return (0);
}
