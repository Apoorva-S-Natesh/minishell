/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:02:09 by asomanah          #+#    #+#             */
/*   Updated: 2024/10/29 14:32:55 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_variable(t_shell *mini, char *key, char *value, char *arg);
static void	print_invalid_error(char *arg);
static void	handle_export_arg(t_shell *mini, char *arg);

//Sets or updates an environment variable
int	set_env_variable(t_shell *mini, const char *key, const char *value)
{
	t_env	*current;

	current = mini->env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			if (value)
				current->value = ft_strdup(value);
			else
				current->value = NULL;
			return (0);
		}
		current = current->next;
	}
	if (value)
		append_node(&mini->env, (char *)key, ft_strdup(value));
	else
		append_node(&mini->env, (char *)key, NULL);
	return (0);
}

// Prints an error on invalid identifier
static void	print_invalid_error(char *arg)
{
	ft_putstr_fd("Minihell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

// Proesses a single export argument
static void	handle_export_arg(t_shell *mini, char *arg)
{
	char	*eq_pos;
	char	*key;
	char	*value;

	eq_pos = ft_strchr(arg, '=');
	if (eq_pos)
	{
		key = ft_substr(arg, 0, eq_pos - arg);
		if (*(eq_pos + 1) == '\0')
			value = ft_strdup("");
		else if (*(eq_pos + 1) == ' ' && *(eq_pos + 2) == '\0')
			value = ft_strdup(" ");
		else
			value = ft_strdup(eq_pos + 1);
	}
	else
	{
		key = ft_strdup(arg);
		value = NULL;
	}
	if (key)
		handle_variable(mini, key, value, arg);
	free(key);
	free(value);
}

static void	handle_variable(t_shell *mini, char *key, char *value, char *arg)
{
	if (!is_valid_identifier((key)))
	{
		print_invalid_error(arg);
		mini->last_exit_status = 1;
	}
	else if (set_env_variable(mini, key, value) != 0)
	{
		ft_putstr_fd("minishell: export: unable to set variable\n", 2);
		mini->last_exit_status = 1;
	}
}

void	builtin_export(char **tokens, t_shell *mini)
{
	int	i;

	i = 1;
	if (!tokens[1])
	{
		print_exported_vars(mini->env);
		mini->last_exit_status = 0;
		return ;
	}
	while (tokens[i])
	{
		if (ft_strchr(tokens[i], '='))
			handle_export_arg(mini, tokens[i]);
		else if (is_valid_identifier(tokens[i]))
			set_env_variable(mini, tokens[i], NULL);
		else
		{
			print_invalid_error(tokens[i]);
			mini->last_exit_status = 1;
		}
		i++;
	}
	if (mini->last_exit_status != 1)
		mini->last_exit_status = 0;
}
