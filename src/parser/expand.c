/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:59:46 by aschmidt          #+#    #+#             */
/*   Updated: 2024/11/15 10:29:04 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand_value(char *token, t_shell *mini)
{
	t_expand_info	info;

	initialize_info(&info, token, mini);
	info.result = malloc(info.size);
	if (!info.result)
		return (NULL);
	while (*info.tkn_ptr)
	{
		if (*info.tkn_ptr == '$' && *(info.tkn_ptr + 1) \
			&& !ft_isspace(*(info.tkn_ptr + 1)))
		{
			if (!handle_dollar_sign(&info))
				return (NULL);
		}
		else
		{
			if (!append_char(&info, *info.tkn_ptr))
				return (NULL);
			info.tkn_ptr++;
		}
	}
	info.result[info.len] = '\0';
	return (info.result);
}

static int	expand_token_value(t_token *token, t_shell *mini)
{
	char	*expanded;

	if (token->type != SINGLE_Q && ft_strchr(token->value, '$'))
	{
		expanded = expand_value(token->value, mini);
		if (expanded)
		{
			if (*expanded == '\0')
			{
				free(expanded);
				return (0);
			}
			free(token->value);
			token->value = expanded;
		}
	}
	return (1);
}

void	expand_tokens(t_token **tokens, t_shell *mini)
{
	t_token	*current;
	t_token	*prev;
	t_token	*temp;

	current = *tokens;
	prev = NULL;
	while (current)
	{
		if (!expand_token_value(current, mini))
		{
			if (prev)
				prev->next = current->next;
			else
				*tokens = current->next;
			temp = current;
			current = current->next;
			free(temp);
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}

char	*extract_env(char **ptr, t_shell *mini)
{
	char	*var_start;
	size_t	var_length;
	char	*var_name;
	char	*value;

	var_start = *ptr;
	if (**ptr == '?')
	{
		(*ptr)++;
		return (ft_itoa(mini->last_exit_status));
	}
	while (**ptr && (ft_isalnum(**ptr) || **ptr == '_'))
		(*ptr)++;
	var_length = *ptr - var_start;
	var_name = malloc(var_length + 1);
	if (!var_name)
		return (NULL);
	ft_strlcpy(var_name, var_start, var_length + 1);
	value = get_env_value(var_name, mini->env);
	free (var_name);
	return (value);
}

char	*get_env_value(const char *name, t_env *env_list)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->key, name) == 0)
			return (ft_strdup(current->value));
		current = current->next;
	}
	return (NULL);
}
