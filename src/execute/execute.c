/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:51:27 by asomanah          #+#    #+#             */
/*   Updated: 2024/11/14 21:01:43 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	execute(t_shell *mini)
{
	t_process			prcs;
	t_command			*cmd;
	t_pipe_info			pipe_info;
	struct sigaction	sa_ignore;
	struct sigaction	sa_old;

	mini->redir_info.tempin = dup(STDIN_FILENO);
	mini->redir_info.tempout = dup(STDOUT_FILENO);
	cmd = mini->commands;
	initialize_process(&prcs);
	initialize_pipe_info(&pipe_info);
	while (cmd != NULL)
	{
		sa_ignore.sa_handler = SIG_IGN;
		sigemptyset(&sa_ignore.sa_mask);
		sa_ignore.sa_flags = 0;
		sigaction(SIGINT, &sa_ignore, &sa_old);
		execute_single_command(cmd, &prcs, mini, &pipe_info);
		sigaction(SIGINT, &sa_old, NULL);
		cleanup_redirections(&prcs);
		cmd = cmd->next;
	}
	finish_execution(mini);
}

void	execute_single_command(t_command *cmd, t_process *prcs, \
t_shell *mini, t_pipe_info *pipe_info)
{
	int	redir_result;

	if (cmd->next)
	{
		if (create_pipe(pipe_info->pipe_fd) == -1)
			return ;
	}
	else
	{
		pipe_info->pipe_fd[0] = -1;
		pipe_info->pipe_fd[1] = -1;
	}
	redir_result = setup_redirs(cmd, prcs, &mini->redir_info, mini);
	if (redir_result <= 0)
	{
		handle_redir_error(redir_result, mini);
		return ;
	}
	if (is_builtin(cmd))
		execute_builtin(cmd, mini, pipe_info);
	else if (cmd->tokens && cmd->tokens[0])
		execute_non_builtin(cmd, prcs, mini, pipe_info);
	handle_exection_pipes(pipe_info, cmd);
}

void	execute_builtin(t_command *cmd, t_shell *mini, t_pipe_info *pipe_info)
{
	int	stdout_copy;
	int	stdin_copy;
	int	in_pipeline;

	stdout_copy = dup(STDOUT_FILENO);
	stdin_copy = dup(STDIN_FILENO);
	if (pipe_info->prev_pipe[0] != -1 || cmd->next != NULL)
		in_pipeline = 1;
	else
		in_pipeline = 0;
	if (pipe_info->prev_pipe[0] != -1)
	{
		dup2(pipe_info->prev_pipe[0], STDIN_FILENO);
		close(pipe_info->prev_pipe[0]);
	}
	if (cmd->next && pipe_info->pipe_fd[1] != -1)
	{
		dup2(pipe_info->pipe_fd[1], STDOUT_FILENO);
	}
	handle_builtin(cmd, mini, in_pipeline);
	dup2(stdout_copy, STDOUT_FILENO);
	dup2(stdin_copy, STDIN_FILENO);
	close(stdout_copy);
	close(stdin_copy);
}

void	handle_parent_process(t_exec_info *exec_info)
{
	if (exec_info->pipe_info.prev_pipe[0] != -1)
	{
		close(exec_info->pipe_info.prev_pipe[0]);
		exec_info->pipe_info.prev_pipe[0] = -1;
	}
	if (exec_info->cmd->next != NULL)
	{
		if (exec_info->pipe_info.prev_pipe[1] != -1)
			close(exec_info->pipe_info.prev_pipe[1]);
		exec_info->pipe_info.prev_pipe[0] = exec_info->pipe_info.pipe_fd[0];
		exec_info->pipe_info.prev_pipe[1] = exec_info->pipe_info.pipe_fd[1];
	}
	else
	{
		if (exec_info->pipe_info.prev_pipe[1] != -1)
		{
			close(exec_info->pipe_info.prev_pipe[1]);
			exec_info->pipe_info.prev_pipe[1] = -1;
		}
		wait_for_child(exec_info->prcs);
		handle_child_status(exec_info->prcs, exec_info->mini);
	}
}

void	handle_exection_pipes(t_pipe_info *pipe_info, t_command *cmd)
{
	if (pipe_info->prev_pipe[0] != -1)
	{
		close(pipe_info->prev_pipe[0]);
		pipe_info->prev_pipe[0] = -1;
	}
	if (cmd->next)
	{
		if (pipe_info->prev_pipe[1] != -1)
			close(pipe_info->prev_pipe[1]);
		pipe_info->prev_pipe[0] = pipe_info->pipe_fd[0];
		pipe_info->prev_pipe[1] = pipe_info->pipe_fd[1];
	}
	else
	{
		if (pipe_info->prev_pipe[1] != -1)
		{
			close(pipe_info->prev_pipe[1]);
			pipe_info->prev_pipe[1] = -1;
		}
	}
}
