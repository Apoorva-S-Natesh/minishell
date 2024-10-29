/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 13:33:37 by aschmidt          #+#    #+#             */
/*   Updated: 2024/10/29 14:30:38 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_free(char **arr)
{
	int	i;

	i = 0;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	free_tokens(t_token *tokens)
{
	t_token	*temp;

	while (tokens)
	{
		temp = tokens;
		tokens = tokens->next;
		free(temp->value);
		free(temp);
	}
}


void	free_env_array(char **env_array)
{
	int	i;

	i = 0;
	while (env_array[i])
	{
		free(env_array[i]);
		i++;
	}
	free(env_array);
}

void	free_redirections(t_redirection *redirection)
{
	t_redirection	*tmp;

	while (redirection)
	{
		tmp = redirection;
		redirection = redirection->next;
		free(tmp);
	}
}

void	free_command(t_command *command)
{
	t_command	*tmp;
	int			i;

	while (command)
	{
		tmp = command;
		command = command->next;
		if (tmp->tokens)
		{
			i = 0;
			while (tmp->tokens[i] != NULL)
			{
				free(tmp->tokens[i]); // Free token strings
				i++;
			}
			free(tmp->tokens); // Free the tokens array itself
		}
		free_redirections(tmp->redirection);
		free(tmp);
	}
}
