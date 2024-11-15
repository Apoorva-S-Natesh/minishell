/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_value.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:24:35 by asomanah          #+#    #+#             */
/*   Updated: 2024/11/14 23:50:51 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	resize_result(t_expand_info *info, size_t needed)
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

int	append_var_value(t_expand_info *info, char *value)
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

int	handle_dollar_sign(t_expand_info *info)
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

int	append_char(t_expand_info *info, char c)
{
	if (info->len + 1 >= info->size)
	{
		if (!resize_result(info, info->len + 2))
			return (0);
	}
	info->result[info->len++] = c;
	return (1);
}

void	initialize_info(t_expand_info *info, char *token, t_shell *mini)
{
	info->size = ft_strlen(token) * 2;
	info->len = 0;
	info->tkn_ptr = token;
	info->mini = mini;
}
