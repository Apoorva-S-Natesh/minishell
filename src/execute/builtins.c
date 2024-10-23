/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:51:04 by asomanah          #+#    #+#             */
/*   Updated: 2024/10/01 16:59:56 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	is_builtin(t_command *cmd)
{
	if (!cmd || !cmd->tokens || !cmd->tokens[0])
		return 0;
	if ((strcmp(cmd->tokens[0], "echo") == 0) || 
		(strcmp(cmd->tokens[0], "cd") == 0) || 
		(strcmp(cmd->tokens[0], "pwd") == 0) || 
		(strcmp(cmd->tokens[0], "export") == 0) || 
		(strcmp(cmd->tokens[0], "unset") == 0) || 
		(strcmp(cmd->tokens[0], "env") == 0) || 
		(strcmp(cmd->tokens[0], "exit") == 0))
	{
		return (1);
	}
	return (0);
}

 //change to libft strcmp (now string.h strcmp)
void	handle_builtin(t_command *cmd, t_shell *mini)
{
	int	size;

	size = 0;
	while (cmd->tokens[size] != NULL)
		size++;
	if (strcmp(cmd->tokens[0], "echo") == 0)
		builtin_echo(cmd->tokens, mini, size);
	if (strcmp(cmd->tokens[0], "cd") == 0)
		builtin_cd(cmd->tokens, mini, size);
	if (strcmp(cmd->tokens[0], "pwd") == 0)
		builtin_pwd(mini);
	if (strcmp(cmd->tokens[0], "export") == 0)
		builtin_export(cmd->tokens, mini);
	if (strcmp(cmd->tokens[0], "unset") == 0)
		builtin_unset(mini, cmd->tokens);
	if (strcmp(cmd->tokens[0], "env") == 0)
		builtin_env(mini, size);
	if (strcmp(cmd->tokens[0], "exit") == 0)
		builtin_exit(cmd->tokens, mini, size);
}
