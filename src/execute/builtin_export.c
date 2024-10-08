/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:02:09 by asomanah          #+#    #+#             */
/*   Updated: 2024/10/03 14:10:24 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
static void	handle_variable(t_shell *mini, char *key, char *value, char *arg);
static void	print_exported_vars(t_env *env);
static int	set_env_variable(t_shell *mini, const char *key, const char *value);
static void	print_invalid_error(char *arg);
static void	handle_export_arg(t_shell *mini, char *arg);

// Checks if a string is a valid variable identifier.

int	is_valid_identifier(const char *str)
{
	if (!str || !*str || ft_isdigit(*str))
		return (0);
	while (*str)
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (0);
		str++;
	}
	return (1);
}

// Prints all exported variable.

static void	print_exported_vars(t_env *env)
{
	while (env)
	{
		ft_putstr_fd("export ", 1);
		ft_putstr_fd(env->key, 1);
		if (env->value)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(env->value, 1);
			ft_putstr_fd("\"", 1);
		}
		ft_putstr_fd("\n", 1);
		env = env->next;
	}
}

//Sets or updates an environment variable

static int	set_env_variable(t_shell *mini, const char *key, const char *value)
{
	t_env	*current;

	current = mini->env;
	while (current)
	{
		if (strcmp(current->key, key) == 0) // change to ft_strncmp
		{
			free(current->value);
			if (value)
				current->value= ft_strdup(value);
			else
				current->value = NULL;
			return (0);
		}
		current = current->next;
	}
	append_node(&mini->env, (char *)key, (char *)value);
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

	eq_pos = ft_strrchr(arg, '=');
	if (eq_pos)
	{
		key = ft_substr(arg, 0, eq_pos - arg);
		if (*(eq_pos + 1) == '\0' || *(eq_pos + 1) == ' ')
			value = ft_strdup(" ");
		else
			value = ft_strdup(eq_pos + 1);
	}
	else // If there is no value, only key and no '='
	{
		key = ft_strdup(arg);
		value = NULL;
	}
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

void builtin_export(char **tokens, t_shell *mini)
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
		handle_export_arg(mini, tokens[i]);
		i++;
    }
	if (mini->last_exit_status != 1) //to make sure that we don't overwrite an error status (1) with a success status (0)
		mini->last_exit_status = 0;
}
