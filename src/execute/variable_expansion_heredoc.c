#include "../../includes/minishell.h"

// Helper function to get the length of a variable name
static int get_var_name_length(const char *str)
{
    int len = 0;
    if (*str == '{')
    {
        str++;
        while (*str && *str != '}')
        {
            len++;
            str++;
        }
    }
    else
    {
        while (isalnum(*str) || *str == '_')
        {
            len++;
            str++;
        }
    }
    return len;
}

// Helper function to get the value of a variable
static char *get_var_value(const char *var_name, t_shell *mini)
{
    if (strcmp(var_name, "?") == 0)
    {
        // Handle $? special variable
        char *exit_status = ft_itoa(mini->last_exit_status);
        return exit_status;
    }

    // Look up the variable in the environment
    t_env *env = mini->env;
    while (env)
    {
        if (strcmp(env->key, var_name) == 0)
            return ft_strdup(env->value);
        env = env->next;
    }

    // Variable not found
    return (ft_strdup(""));
}

char *expand_variables(char *line, t_shell *mini)
{
    char *result = NULL;
    int result_len = 0;
    int result_capacity = 0;
    char *p = line;

    while (*p)
    {
        if (*p == '$' && *(p + 1) != '\0' && *(p + 1) != ' ')
        {
            // Variable expansion
            p++;
            int var_name_len = get_var_name_length(p);
            char *var_name = ft_substr(p, 0, var_name_len);
            if (*p == '{')
            {
                var_name = ft_substr(p + 1, 0, var_name_len - 2);
                p += var_name_len + 2;  // Skip ${VAR}
            }
            else
            {
                var_name = ft_substr(p, 0, var_name_len);
                p += var_name_len;  // Skip $VAR
            }

            char *var_value = get_var_value(var_name, mini);
            int value_len = strlen(var_value);

            // Ensure result has enough capacity
            if (result_len + value_len >= result_capacity)
            {
                result_capacity = result_capacity * 2 + value_len;
                result = realloc(result, result_capacity);
            }

            // Append variable value to result
            ft_memcpy(result + result_len, var_value, value_len);
            result_len += value_len;

            free(var_name);
            free(var_value);
        }
        else
        {
            // Regular character
            if (result_len + 1 >= result_capacity)
            {
                result_capacity = result_capacity * 2 + 1;
                result = realloc(result, result_capacity);
            }
            result[result_len++] = *p++;
        }
    }

    // Null-terminate the result
    if (result_len + 1 >= result_capacity)
    {
        result_capacity++;
        result = realloc(result, result_capacity);
    }
    result[result_len] = '\0';

    return (result);
}

/*
It iterates through the input string character by character.
When it encounters a $ followed by a valid variable name character, it starts the variable expansion process:
It determines the length of the variable name.
It extracts the variable name.
It looks up the value of the variable in the shell's environment.
It appends the variable's value to the result string.
For non-variable characters, it simply appends them to the result string.
It handles both $VAR and ${VAR} syntax.
It specially handles the $? variable, which expands to the last exit status.
It dynamically allocates and resizes the result string as needed.
*/