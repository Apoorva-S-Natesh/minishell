#include "../../includes/minishell.h"

void expand_tokens(t_token *tokens, t_env *env_list)
{
	t_token	*current;
	char	*expanded;
	char *cleaned_value;

	current = tokens;
	while (current)
	{
		if (current->type != SINGLE_Q)
		{
			if (ft_strchr(current->value, '$'))
			{
				expanded = expand_value(current->value, env_list);
				free(current->value);
				current->value = expanded;
			}
		}
		cleaned_value = remove_quotes(current->value);
		free(current->value);
		current->value = cleaned_value;
		current = current->next;
	}
}

static char *expand_var(char **tkn_ptr, t_env *env_list, char *result_ptr, size_t *current_length, size_t max_size)
{
    char	*env_value;
	size_t	env_length;

	env_value = extract_env(tkn_ptr, env_list);
	if (env_value)
	{
		env_length = strlen(env_value);
		if (*current_length + env_length >= max_size)
		{
			printf("Error: Result buffer size exceeded\n");
			return (NULL);
		}
		ft_strlcpy(result_ptr + *current_length, env_value, ft_strlen(env_value) + 1);
		*current_length += env_length; // Update the current length
	}
	return (env_value);
}

char	*expand_value(char	*token, t_env *env_list)
{
	char	*result; //new string with variables expanded
	char	*tkn_ptr;
	size_t	current_length;
	size_t	max_size;

	max_size = 1024;
	current_length = 0;
	result = malloc(max_size); //space enough to store expanded vars
	if (!result)
		return (NULL);
	result[0] = '\0';
	tkn_ptr = token;
	while (*tkn_ptr)
	{
		if (*tkn_ptr == '$')
		{
			tkn_ptr++; //skip the sign
			printf("Expanding variable at: %s\n", tkn_ptr);
			expand_var(&tkn_ptr, env_list, result, &current_length, max_size);
		}
		else
			result[current_length++] = *tkn_ptr++; //if theres no dollar sign, just copy the char;
	}
	result[current_length] = '\0';
	return (result);
}

char *extract_env(char **ptr, t_env *env_list)
{
	char	*var_start;
	size_t	var_length;
	char	*var_name;
	char	*value;

	var_start = *ptr;
	while (**ptr && (ft_isalnum(**ptr) || **ptr == '_')) //envv van have only alnum char or _
		(*ptr)++;
	var_length = *ptr - var_start;
	var_name = malloc(var_length + 1);
	if (!var_name)
		return (NULL);
	ft_strlcpy(var_name, var_start, var_length + 1);
	value = get_env_value(var_name, env_list);
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
			return (current->value);
		current = current->next;
	}
	return (NULL);
}
