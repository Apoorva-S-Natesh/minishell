#include "../../includes/minishell.h"

char *remove_quotes(char *str)
{
    char *cleaned_str;
    char *src;
	char *dst;
    size_t len;

	len = ft_strlen(str);
    cleaned_str = malloc(len + 1);
    if (!cleaned_str)
        return NULL;
    src = str;
    dst = cleaned_str;
    while (*src)
    {
        if (*src != '\'' && *src != '\"')
            *dst++ = *src;
        src++;
    }
    *dst = '\0'; // Null-terminate the cleaned string
    return (cleaned_str);
}

char *extract_quoted_content(char *input, int *i, int quote_type, int buffer_index)
{
	char	*buffer;
	char	closing_quote;

	buffer = malloc(256);
	if (buffer == NULL)
		return (NULL);
	(*i)++; // Move past the opening quote
	if (quote_type == SINGLE_Q)
		closing_quote = '\'';
	else
		closing_quote = '\"';
    while (input[*i] && input[*i] != closing_quote) // Extract content until the closing quote
	{
		buffer[buffer_index++] = input[*i];
		(*i)++;
	}
    if (input[*i] == closing_quote)
	{
        buffer[buffer_index] = '\0'; // Null-terminate the buffer
        (*i)++; // Move past the closing quote
        return (buffer); // Return the extracted content
    }
    free(buffer); // Free memory if unclosed quote
    return (NULL); // Return NULL for unclosed quotes
}

int handle_quotes(t_shell *mini, int *i, t_token **tokens)
{
	int		quote_type;
	char	*quoted_content;
	char	*expanded_content;
	int		buffer_index; //just to not exceed 25 lines in the next function

	buffer_index = 0;
	if (mini->input[*i] == '\'')
		quote_type = SINGLE_Q;
	else
		quote_type = DOUBLE_Q;
	quoted_content = extract_quoted_content(mini->input, i, quote_type, buffer_index);
	if (quoted_content == NULL)
	{
		printf("Error: Unclosed quote detected!\n");
		return (0); // Indicate an error
	}
	if (quote_type == DOUBLE_Q) // Expand for double quotes, but directly append for single quotes
	{
		expanded_content = expand_value(quoted_content, mini);
		append_token(tokens, expanded_content, DOUBLE_Q, 2); // Append expanded token
		free(expanded_content); // Free after use
	}
	else // Directly append single-quoted content without expansion
		append_token(tokens, quoted_content, SINGLE_Q, 1);
	free(quoted_content);
	return (1); // Successful execution
}
