/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_set_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:14:52 by asomanah          #+#    #+#             */
/*   Updated: 2024/11/14 15:24:31 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	set_env_variable(t_shell *mini, const char *key, const char *value)
{
	t_env	*current;
	char	*new_value;
	char	*new_key;

	current = mini->env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (current->value)
				free(current->value);
			if (value)
				new_value = ft_strdup(value);
			else
				new_value = NULL;
			current->value = new_value;
			return (0);
		}
		current = current->next;
	}
	new_key = ft_strdup(key);
	if (value)
		new_value = ft_strdup(value);
	else
		new_value = NULL;
	if (!new_key || (value && !new_value))
	{
		free(new_key);
		free(new_value);
		return (1);
	}
	append_node(&mini->env, new_key, new_value);
	free(new_key);
	free(new_value);
	return (0);
}
