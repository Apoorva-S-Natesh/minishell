#include <stddef.h>
#include <string.h>

char *ft_strtok(char *str, const char *delim)
{
    static char *last_token = NULL;
    char *token_start;
    
    // If str is NULL, continue from last position
    if (str == NULL)
        str = last_token;

    // Return NULL if we've reached the end of the string
    if (str == NULL || *str == '\0')
        return NULL;

    // Skip leading delimiters
    str += strspn(str, delim);
    if (*str == '\0')
    {
        last_token = NULL;
        return NULL;
    }

    // Find the end of the token
    token_start = str;
    str = strpbrk(str, delim);
    if (str == NULL)
    {
        // This is the last token
        last_token = NULL;
    }
    else
    {
        // Null-terminate the token and set last_token to the next character
        *str = '\0';
        last_token = str + 1;
    }

    return token_start;
}