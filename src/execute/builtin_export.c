/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:02:09 by asomanah          #+#    #+#             */
/*   Updated: 2024/10/01 17:23:56 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Prints all exported variables in the format declare -x NAME="VALUE" if no arguments are provided.
Processes each argument:
Splits the argument into key and value (if there's an '=' sign).
Checks if the key is a valid identifier.
Sets or updates the environment variable.
Handles errors and sets the appropriate exit status.
*/
#include "../../includes/minishell.h"

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

/*
static int is_valid_identifier(const char *str)
{
    if (!str || !*str || isdigit(*str))
        return 0;
    
    while (*str)
    {
        if (!isalnum(*str) && *str != '_')
            return 0;
        str++;
    }
    return 1;
}

static void print_exported_vars(t_env *env)
{
    while (env)
    {
        ft_putstr_fd("declare -x ", 1);
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

static int set_env_var(t_shell *shell, const char *key, const char *value)
{
    t_env *current = shell->env;
    t_env *new_var;

    while (current)
    {
        if (strcmp(current->key, key) == 0)
        {
            free(current->value);
            current->value = value ? ft_strdup(value) : NULL;
            return 0;
        }
        current = current->next;
    }

    new_var = new_env(key, value);
    if (!new_var)
        return 1;
    
    append_node(&shell->env, key, value);
    return 0;
}

void builtin_export(t_shell *shell, char **args)
{
    int i = 1;
    char *eq_pos;
    char *key;
    char *value;

    if (!args[1])
    {
        print_exported_vars(shell->env);
        shell->last_exit_status = 0;
        return;
    }

    while (args[i])
    {
        eq_pos = strchr(args[i], '=');
        if (eq_pos)
        {
            key = ft_substr(args[i], 0, eq_pos - args[i]);
            value = ft_strdup(eq_pos + 1);
        }
        else
        {
            key = ft_strdup(args[i]);
            value = NULL;
        }

        if (!is_valid_identifier(key))
        {
            ft_putstr_fd("minishell: export: `", 2);
            ft_putstr_fd(args[i], 2);
            ft_putstr_fd("': not a valid identifier\n", 2);
            shell->last_exit_status = 1;
        }
        else
        {
            if (set_env_var(shell, key, value) != 0)
            {
                ft_putstr_fd("minishell: export: unable to set variable\n", 2);
                shell->last_exit_status = 1;
            }
        }

        free(key);
        free(value);
        i++;
    }

    if (shell->last_exit_status != 1)
        shell->last_exit_status = 0;
}
*/