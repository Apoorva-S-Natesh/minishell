#include "../../includes/minishell.h"

static int is_special_token(t_token *token)
{
    if (token->type == PIPE || token->type == RED_IN ||\
	 token->type == RED_OUT || token->type == APPEND || token->type == HEREDOC)
		return (1);
	else
		return (0);
}

int validate_tokens(t_token *tokens)
{
    t_token *current;

	current = tokens;
    if (current && current->type == PIPE)
        return (0);
    while (current && current->next)
    {
        if (is_special_token(current) && is_special_token(current->next))
            return (0);
        current = current->next;
    }
    if (current && current->type == PIPE)
        return (0);
    return (1);
}
