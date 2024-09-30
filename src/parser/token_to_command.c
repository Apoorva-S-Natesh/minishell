#include "../../includes/minishell.h"

void	process_token(t_command **current_cmd, t_token **current_tkn)
{
	e_token_type	type;

	type = (*current_tkn)->type;
	if (type == PIPE)
		*current_cmd = NULL;
	else
	{
		add_tkn_to_cmd(*current_cmd, *current_tkn);
		if (type == RED_IN || type == RED_OUT \
			|| type == APPEND || type == HEREDOC)
		{
			handle_redirection(current_cmd, current_tkn);
		}
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






