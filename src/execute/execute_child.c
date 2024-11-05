/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:24:35 by asomanah          #+#    #+#             */
/*   Updated: 2024/11/01 13:20:48 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_child_status(t_process *prcs, t_shell *mini)
{
	if (WIFEXITED(prcs->status))
	{
		prcs->exit_code = WEXITSTATUS(prcs->status);
		mini->last_exit_status = prcs->exit_code;
	}
	else if (WIFSIGNALED(prcs->status))
	{
		prcs->signal = WTERMSIG(prcs->status);
		mini->last_exit_status = 128 + prcs->signal;
	}
	if (prcs->signal == SIGQUIT)
		ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
	else if (prcs->signal == SIGINT)
		ft_putstr_fd("\n", STDERR_FILENO);
	free(prcs->cmd_path);
	prcs->cmd_path = NULL;
}

int	handle_child_process(t_exec_info *exec_info)
{
	int	err_no;

	setup_child_signals();
	if (exec_info->pipe_info.prev_pipe[0] != -1)
	{// Redirect stdin to the read end of the previous pipe
		if (dup2(exec_info->pipe_info.prev_pipe[0], STDIN_FILENO) == -1)
			perror("dup2");
		close(exec_info->pipe_info.prev_pipe[0]);
	}
	if (exec_info->pipe_info.prev_pipe[1] != -1)
		close(exec_info->pipe_info.prev_pipe[1]);
	if (exec_info->cmd->next != NULL && exec_info->pipe_info.pipe_fd[1] != -1)
	{// Redirect stdout to the write end of the current pipe
		if (dup2(exec_info->pipe_info.pipe_fd[1], STDOUT_FILENO) == -1)
			perror ("dup2");
	}
	if (exec_info->pipe_info.pipe_fd[0] != -1)
		close(exec_info->pipe_info.pipe_fd[0]);
	if (exec_info->pipe_info.pipe_fd[0] != -1)
		close(exec_info->pipe_info.pipe_fd[1]);
	close(exec_info->mini->signal_pipe[0]);
	close(exec_info->mini->signal_pipe[1]);
	err_no = execute_command(exec_info->cmd, exec_info->prcs, exec_info->mini);
	return (err_no);
}

void	wait_for_child(t_process *prcs, t_shell *mini)
{
	fd_set			readfds;
	struct timeval	tv;
	int				ret;
	char			sig;

	while (1)
	{
		FD_ZERO(&readfds);
		FD_SET(mini->signal_pipe[0], &readfds);
		tv.tv_sec = 0;
		tv.tv_usec = 100000;
		ret = select(mini->signal_pipe[0] + 1, &readfds, NULL, NULL, &tv);
		if (ret == -1 && errno != EINTR)
		{
			perror("select");
			break ;
		}
		else if (ret > 0 && (read(mini->signal_pipe[0], &sig, 1)) > 0 && (sig == SIGINT))
		{
			kill(prcs->pid, SIGINT);
			write(STDOUT_FILENO, "\n", 1);
		}
		if (waitpid(prcs->pid, &prcs->status, WNOHANG) != 0)
			break ;
	}
}
