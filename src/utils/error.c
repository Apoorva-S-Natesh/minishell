#include "../../includes/minishell.h"

int is_special_token(t_token *token)
{
    if (token->type == PIPE || token->type == RED_IN \
		|| token->type == RED_OUT || token->type == APPEND \
		|| token->type == HEREDOC)
		return (1);
	else
	return (0);
}

int check_invalid_tokens(t_token *tokens)
{
    t_token *prev;

    prev = NULL;
    if (tokens && tokens->type == PIPE)
    {
        printf("syntax error near unexpected token `|'\n");
        return (0);
    }
    while (tokens != NULL)
    {
        if (tokens->next == NULL && tokens->type == PIPE)// Check if the current token is a pipe and it's the last token
        {
            printf("syntax error near unexpected token `|'\n");
            return (0);
        }
        if (prev && is_special_token(prev) && is_special_token(tokens))
        {
            printf("Error: Consecutive special tokens '%s' and '%s'.\n", prev->value, tokens->value);
            return (0);
        }
        prev = tokens;
        tokens = tokens->next;
    }
    return (1);
}
