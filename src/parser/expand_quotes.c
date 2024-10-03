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

