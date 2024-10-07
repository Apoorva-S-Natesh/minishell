/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 12:11:17 by asomanah          #+#    #+#             */
/*   Updated: 2024/06/12 17:03:03 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t			total;
	size_t			i;
	unsigned char	*temp;

	total = nmemb * size;
	i = 0;
	if (nmemb && size && nmemb > ((size_t) - 1 / size))
		return (NULL);
	temp = malloc(total);
	if (!temp)
		return (NULL);
	while (i < total)
	{
		temp[i] = 0;
		i++;
	}
	return (temp);
}

char	*ft_str_join(char *result, char *buff)
{
	char	*temp;

	temp = ft_strjoin (result, buff);
	free (result);
	return (temp);
}