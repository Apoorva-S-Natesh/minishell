/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_set_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 19:47:27 by asomanah          #+#    #+#             */
/*   Updated: 2024/11/14 21:13:32 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Function to update an existing environment variable
int	update_env_variable(t_env *current, const char *value)
{
	if (current->value)
	{
		free(current->value);
	}
	if (value)
	{
		current->value = ft_strdup(value);
	}
	else
	{
		current->value = NULL;
	}
	return (0);
}

// Function to create a new key-value pair for the environment
int	create_new_env_variable(t_shell *mini, const char *key, const char *value)
{
	char	*new_key;
	char	*new_value;

	new_key = ft_strdup(key);
	if (value)
	{
		new_value = ft_strdup(value);
	}
	else 
		new_value = NULL;
	if (!new_key || (value && !new_value)) 
	{
		free(new_key);
		if (new_value) 
		{
			free(new_value);
		}
		return (1);
	}
	append_node(&mini->env, new_key, new_value);
	free(new_key);
	free(new_value);
	return (0);
}

// Main function to set an environment variable
int	set_env_variable(t_shell *mini, const char *key, const char *value)
{
	t_env	*current;

	current = mini->env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			return (update_env_variable(current, value));
		}
		current = current->next;
	}
	return (create_new_env_variable(mini, key, value));
}
