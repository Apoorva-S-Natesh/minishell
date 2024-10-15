#include "../../includes/minishell.h"

void expand_tokens(t_token *tokens, t_shell *mini)
{
    t_token *current;
	char *expanded;

	current = tokens;
    while (current)
	{
        if (current->type != SINGLE_Q && ft_strchr(current->value, '$'))
		{
            expanded = expand_value(current->value, mini);
            if (expanded)
			{
                free(current->value);
                current->value = expanded;
            }
        }
        current = current->next;
    }
}


char *expand_value(char *token, t_shell *mini)
{
    char *result = malloc(1024);
    if (!result) return NULL;

    char *tkn_ptr = token;
    size_t current_length = 0;

    while (*tkn_ptr) {
        if (*tkn_ptr == '$') {
            tkn_ptr++; // Skip the dollar sign
            char *var_value = extract_env(&tkn_ptr, mini);
            if (var_value) {
                size_t var_length = ft_strlen(var_value);
                if (current_length + var_length >= 1024) {
                    // Handle overflow here (e.g., realloc)
                }
                ft_strlcpy(result + current_length, var_value, var_length + 1);
                current_length += var_length;
                free(var_value);
            }
        } else {
            result[current_length++] = *tkn_ptr++;
        }
    }
    result[current_length] = '\0';
    return result;
}

char *extract_env(char **ptr, t_shell *mini)
{
	char	*var_start;
	size_t	var_length;
	char	*var_name;
	char	*value;

	var_start = *ptr;
	if (**ptr == '?' && *(*ptr + 1) == '\0')
	{
		(*ptr)++;
        return (ft_itoa(mini->last_exit_status));
	}
	while (**ptr && (ft_isalnum(**ptr) || **ptr == '_')) //envv van have only alnum char or _
		(*ptr)++;
	var_length = *ptr - var_start;
	var_name = malloc(var_length + 1);
	if (!var_name)
		return (NULL);
	ft_strlcpy(var_name, var_start, var_length + 1);
	value = get_env_value(var_name, mini->env);
    free (var_name);
	return (value);
}

char	*get_env_value(const char *name, t_env *env_list)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->key, name) == 0)
			return (ft_strdup(current->value));
		current = current->next;
	}
	return (NULL);
}
