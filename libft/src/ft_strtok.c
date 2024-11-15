/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 01:26:41 by asomanah          #+#    #+#             */
/*   Updated: 2024/11/15 01:44:35 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_delimiter(char c, const char *delim)
{
	while (*delim)
	{
		if (c == *delim)
			return (1);
		delim++;
	}
	return (0);
}

static char	*skip_delimiters(char *str, const char *delim)
{
	while (*str && is_delimiter(*str, delim))
		str++;
	return (str);
}

static char	*find_token_end(char *str, const char *delim)
{
	while (*str && !is_delimiter(*str, delim))
		str++;
	return (str);
}

char	*ft_strtok(char *str, const char *delim)
{
	static char	*g_last_token;
	char		*token_start;
	char		*token_end;

	g_last_token = NULL;
	if (!str)
		str = g_last_token;
	if (!str || !*str)
		return (NULL);
	str = skip_delimiters(str, delim);
	if (!*str)
		return (NULL);
	token_start = str;
	token_end = find_token_end(str, delim);
	if (*token_end)
	{
		*token_end = '\0';
		g_last_token = token_end + 1;
	}
	else
		g_last_token = NULL;
	return (token_start);
}
