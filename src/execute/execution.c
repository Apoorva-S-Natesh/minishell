/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschmidt <aschmidt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:31:30 by asomanah          #+#    #+#             */
/*   Updated: 2024/11/14 22:05:21 by aschmidt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	execute_command_helper(t_command *cmd, char *cmd_path, \
char **env_array)
{
	execve(cmd_path, cmd->tokens, env_array);
	print_err_msg(cmd->tokens[0], strerror(errno));
	return (126);
}

int	execute_command(t_command *cmd, t_process *prcs, t_shell *mini)
{
	char	**env_array;
	int		status;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	env_array = create_env_array(mini->env);
	prcs->cmd_path = find_command(cmd->tokens[0], mini->env);
	status = check_file_status(prcs->cmd_path);
	if (status != 0)
	{
		handle_command_error(prcs->cmd_path, cmd->tokens[0], status);
		free(prcs->cmd_path);
		free_env_array(env_array);
		return (status);
	}
	status = execute_command_helper(cmd, prcs->cmd_path, env_array);
	free(prcs->cmd_path);
	free_env_array(env_array);
	return (status);
}

int	check_file_status(const char *cmd_path)
{
	struct stat	st;

	if (!cmd_path)
		return (127);
	if (access(cmd_path, F_OK) != 0)
		return (127);
	if (stat(cmd_path, &st) == 0 && S_ISDIR(st.st_mode))
		return (126);
	if (access(cmd_path, X_OK) != 0)
		return (126);
	return (0);
}

void	handle_command_error(const char *cmd_path, const char *cmd_name, int status)
{
	struct stat	st;

	if (status == 127 && !cmd_path)
		print_err_msg(cmd_name, "command not found");
	else if (status == 127)
		print_err_msg(cmd_name, strerror(errno));
	else if (status == 126)
	{
		if (stat(cmd_path, &st) == 0 && S_ISDIR(st.st_mode))
			print_err_msg(cmd_name, "Is a directory");
		else
			print_err_msg(cmd_name, "Permission denied");
	}
}

void	finish_execution(t_shell *mini)
{
	restore_main_signals();
	while (wait(NULL) > 0);
	// Restore original stdin and stdout
	dup2(mini->redir_info.tempin, STDIN_FILENO);
	dup2(mini->redir_info.tempout, STDOUT_FILENO);
	close(mini->redir_info.tempin);
	close(mini->redir_info.tempout);
	close(mini->signal_pipe[0]);
	close(mini->signal_pipe[1]);
}
