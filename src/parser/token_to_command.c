/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_to_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschmidt <aschmidt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 19:34:59 by aschmidt          #+#    #+#             */
/*   Updated: 2024/11/14 19:35:02 by aschmidt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	process_token(t_command **current_cmd, t_token **current_tkn)
{
	e_token_type	type;

	type = (*current_tkn)->type;
	if (type == PIPE)
		*current_cmd = NULL;
	else
	{
		if (type == RED_IN || type == RED_OUT \
			|| type == APPEND || type == HEREDOC)
		{
			if ((*current_tkn)->next && (*current_tkn)->next->type == WORD)
				handle_redirection(current_cmd, current_tkn);
			// else
			// 	printf("minishell: Syntax error: missing file after
			// redirection\n");
		}
		else
			add_tkn_to_cmd(*current_cmd, *current_tkn);
	}
	*current_tkn = (*current_tkn)->next;
}

void	add_tkn_to_cmd(t_command *cmd, t_token *token)
{
	int		i;
	int		j;
	char	**new_tokens;

	i = 0;
	j = 0;
	while (cmd->tokens && cmd->tokens[i]) //calculate existing tokens
		i++;
	new_tokens = malloc(sizeof(char *) * (i + 2)); //create space for existing tokens + new and NULL
	if (!new_tokens)
		return ;
	while (j < i)
	{
		new_tokens[j] = cmd->tokens[j]; //copy old data
		j++;
	}
		new_tokens[i] = ft_strdup(token->value); //add new token
		new_tokens[i + 1] = NULL;
		free(cmd->tokens);
		cmd->tokens = new_tokens;
}
