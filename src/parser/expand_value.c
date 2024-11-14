/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_value.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschmidt <aschmidt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:24:35 by asomanah          #+#    #+#             */
/*   Updated: 2024/11/14 20:55:32 by aschmidt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	resize_result(t_expand_info *info, size_t needed)
{
	char	*new_result;
	size_t	new_size;

	new_size = info->size;
	while (new_size < needed)
		new_size *= 2;
	new_result = malloc(new_size);
	if (!new_result)
	{
		free(info->result);
		info->result = NULL;
		return (0);
	}
	if (info->result)
	{
		ft_memcpy(new_result, info->result, info->size);
		free(info->result);
	}
	info->result = new_result;
	info->size = new_size;
	return (1);
}

static int	append_var_value(t_expand_info *info, char *value)
{
	size_t	value_len;

	value_len = ft_strlen(value);
	if (info->len + value_len >= info->size)
	{
		if (!resize_result(info, info->len + value_len + 1))
			return (0);
	}
	ft_strlcpy(info->result + info->len, value, value_len + 1);
	info->len += value_len;
	return (1);
}

static int	handle_dollar_sign(t_expand_info *info)
{
	char	*var_value;
	int		success;

	info->tkn_ptr++;
	var_value = extract_env(&info->tkn_ptr, info->mini);
	if (var_value)
	{
		success = append_var_value(info, var_value);
		free(var_value);
		return (success);
	}
	return (1);
}


static int	append_char(t_expand_info *info, char c)
{
	if (info->len + 1 >= info->size)
	{
		if (!resize_result(info, info->len + 2))
			return (0);
	}
	info->result[info->len++] = c;
	return (1);
}

char	*expand_value(char *token, t_shell *mini)
{
	t_expand_info	info;

	info.size = ft_strlen(token) * 2;
	info.result = malloc(info.size);
	if (!info.result)
		return (NULL);
	info.len = 0;
	info.tkn_ptr = token;
	info.mini = mini;
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
