/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 16:10:37 by asomanah          #+#    #+#             */
/*   Updated: 2024/11/14 21:40:06 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	initialize_pipe_info(t_pipe_info *pipe_info)
{
	pipe_info->pipe_fd[0] = -1;
	pipe_info->pipe_fd[1] = -1;
	pipe_info->prev_pipe[0] = -1;
	pipe_info->prev_pipe[1] = -1;
}

void	handle_redir_error(int redir_result, t_shell *mini)
{
	if (redir_result == -2)
	{
		mini->last_exit_status = 130;
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	if (redir_result == -1)
	{
		mini->last_exit_status = 1;
	}
}

int	create_pipe(int pipe_fd[2])
{
	int	result;

	result = pipe(pipe_fd);
	if (result == -1)
	{
		perror("pipe");
		return (-1);
	}
	return (0);
}

void	cleanup_pipes(t_command *cmd, t_pipe_info *pipe_info)
{
	if (pipe_info->prev_pipe[0] != -1)
		close(pipe_info->prev_pipe[0]);
	if (pipe_info->prev_pipe[1] != -1)
		close(pipe_info->prev_pipe[1]);
	if (!cmd->next)
	{
		if (pipe_info->pipe_fd[0] != -1)
			close(pipe_info->pipe_fd[0]);
		if (pipe_info->pipe_fd[1] != -1)
			close(pipe_info->pipe_fd[1]);
	}
}

//cleanup function for signals
void	cleanup_signal_pipe(t_shell *mini)
{
	if (mini->signal_pipe[0] != -1)
	{
		close(mini->signal_pipe[0]);
		mini->signal_pipe[0] = -1;
	}
	if (mini->signal_pipe[1] != -1)
	{
		close(mini->signal_pipe[1]);
		mini->signal_pipe[1] = -1;
	}
}
