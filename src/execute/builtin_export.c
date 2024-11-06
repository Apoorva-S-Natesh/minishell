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

static void print_invalid_error(char *arg)
{
    ft_putstr_fd("Minihell: export: `", 2);
    ft_putstr_fd(arg, 2);
    ft_putstr_fd("': not a valid identifier\n", 2);
}

static char *get_key(char *arg)
{
    char *eq_pos;
    char *key;

    eq_pos = ft_strchr(arg, '=');
    if (eq_pos)
        key = ft_substr(arg, 0, eq_pos - arg);
    else
        key = ft_strdup(arg);
    return (key);
}

static char *get_value(char *arg)
{
    char *eq_pos;
    char *value;

    eq_pos = ft_strchr(arg, '=');
    if (eq_pos)
        value = ft_strdup(eq_pos + 1);
    else
        value = NULL;
    return (value);
}

static int handle_export_arg(t_shell *mini, char *arg, int in_pipeline)
{
    char *key;
    char *value;
    int status;

    key = get_key(arg);
    value = get_value(arg);
    status = 0;
    if (!is_valid_identifier(key))
    {
        print_invalid_error(arg);
        status = 1;
    }
    else if (!in_pipeline)
    {
        if (set_env_variable(mini, key, value) != 0)
        {
            ft_putstr_fd("minishell: export: unable to set variable\n", 2);
            status = 1;
        }
    }
	free(key);
    free(value);
    return (status);
}

int set_env_variable(t_shell *mini, const char *key, const char *value)
{
    t_env *current;
    char *new_value;
    char *new_key;

    current = mini->env;
    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
        {
            if (current->value)
                free(current->value);
            new_value = value ? ft_strdup(value) : NULL;
            current->value = new_value;
            return (0);
        }
        current = current->next;
    }
    new_key = ft_strdup(key);
    new_value = value ? ft_strdup(value) : NULL;
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

void builtin_export(char **tokens, t_shell *mini, int in_pipeline)
{
    int i;
    int status;

    i = 1;
    status = 0;
    if (!tokens[1])
    {
        print_exported_vars(mini->env);
        mini->last_exit_status = 0;
        return;
    }
    while (tokens[i])
    {
        status |= handle_export_arg(mini, tokens[i], in_pipeline);
        i++;
    }
    mini->last_exit_status = status;
}
