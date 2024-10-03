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

/*
Prints all exported variables in the format export -x NAME="VALUE" if no arguments are provided.
Processes each argument:
Splits the argument into key and value (if there's an '=' sign).
Checks if the key is a valid identifier.(not a number)
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
    while (*str && *str != '=')
    {
        if (!isalnum(*str) && *str != '_')
            return 0;
        str++;
    }
    return 1;
}

static void print_exported_var(t_env *env)
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
}

static void print_exported_vars(t_env *env)
{
    while (env)
    {
        print_exported_var(env);
        env = env->next;
    }
}

static int set_env_var(t_shell *shell, const char *key, const char *value)
{
    t_env *current = shell->env;
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
    return append_env_var(shell, key, value);
}

static int append_env_var(t_shell *shell, const char *key, const char *value)
{
    t_env *new_var = new_env(key, value);
    if (!new_var)
        return 1;
    append_node(&shell->env, new_var);
    return 0;
}

static void handle_export_arg(t_shell *shell, char *arg)
{
    char *eq_pos = strchr(arg, '=');
    char *key, *value;

    if (eq_pos)
    {
        key = ft_substr(arg, 0, eq_pos - arg);
        value = (*(eq_pos + 1) == '\0' || *(eq_pos + 1) == ' ') 
                ? ft_strdup("") : ft_strdup(eq_pos + 1);
    }
    else
    {
        key = ft_strdup(arg);
        value = NULL;
    }

    if (!is_valid_identifier(key))
    {
        print_invalid_identifier(arg);
        shell->last_exit_status = 1;
    }
    else if (set_env_var(shell, key, value) != 0)
    {
        ft_putstr_fd("minishell: export: unable to set variable\n", 2);
        shell->last_exit_status = 1;
    }

    free(key);
    free(value);
}

void builtin_export(t_shell *shell, char **args)
{
    int i = 1;

    if (!args[1])
    {
        print_exported_vars(shell->env);
        shell->last_exit_status = 0;
        return;
    }

    while (args[i])
    {
        handle_export_arg(shell, args[i]);
        i++;
    }

    if (shell->last_exit_status != 1)
        shell->last_exit_status = 0;
}

static void print_invalid_identifier(char *arg)
{
    ft_putstr_fd("minishell: export: `", 2);
    ft_putstr_fd(arg, 2);
    ft_putstr_fd("': not a valid identifier\n", 2);
}
*/