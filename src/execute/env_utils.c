/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:51:17 by asomanah          #+#    #+#             */
/*   Updated: 2024/10/01 14:51:06 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
typedef struct s_env
{
	char		*key;               // (e.g., "PATH")
	char		*value;             // Path value (e.g., "/usr/bin:/bin")
	t_env		*next;
}	t_env;*/

char	*ft_getenv(const char *name, t_env *env)
{
	while (env)
	{
		if (strcmp(env->key, name) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	append_node(t_env **head, char *key, char *value)
{
	t_env	*new_node;
	t_env	*current;

	new_node = new_env(key, value);
	if (!new_node)
		return ;
	if (*head == NULL)
		*head = new_node;
	else
	{
		current = *head;
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = new_node;
	}
}

t_env	*new_env(char *key, char *value)
{
	t_env	*variable;

	variable = malloc(sizeof(t_env));
	if (!variable)
		return (NULL);
	variable->key = ft_strdup(key);
	variable->value = ft_strdup(value);
	if (!variable->key || !variable->value)
	{
		free(variable->key);
		free(variable->value);
		free(variable);
		return (NULL);
	}
	variable->next = NULL;
	return (variable);
}

void	free_list(t_env *head)
{
	t_env	*current;
	t_env	*next;

	current = head;
	while (current != NULL)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}

void	print_list(t_env *head)
{
	t_env	*current;

	current = head;
	while (current != NULL)
	{
		printf("key: %s, value: %s\n", current->key, current->value);
		current = current->next;
	}
	printf("NULL\n");
}
