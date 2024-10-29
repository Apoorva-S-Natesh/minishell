/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:23:14 by asomanah          #+#    #+#             */
/*   Updated: 2024/10/29 14:23:25 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*The heredoc is typically handled in a child process to allow for proper signal
handling and to prevent the main shell process from being affected by signals
during heredoc input.*/

//Child process function for handling heredoc input
void	heredoc_child_process(int wr_fd, const char *delimiter, t_shell *mini)
{
	if (signal(SIGINT, handle_sigint_heredoc) == SIG_ERR)
	{
		perror("minishell: signal");
		exit(1);
	}
	heredoc_read_loop(wr_fd, delimiter, mini);
	close (wr_fd);
	exit (0);
}

static int	setup_heredoc_pipe(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
	{
		perror("minishell: pipe");
		return (-1);
	}
	return (0);
}

static int	handle_heredoc_parent(t_process *hd_prcs, t_shell *mini)
{
	close(hd_prcs->pipe_fd[1]);
	waitpid(hd_prcs->pid, &hd_prcs->status, 0);
	if (WIFSIGNALED(hd_prcs->status))
	{
		close(hd_prcs->pipe_fd[0]);
		mini->last_exit_status = 128 + WTERMSIG(hd_prcs->status);
		return (-1);
	}
	return (hd_prcs->pipe_fd[0]);
}

static int	fork_heredoc_process(t_process *hd_prcs, const char *delimiter, \
t_shell *mini)
{
	hd_prcs->pid = fork();
	if (hd_prcs->pid == -1)
	{
		perror("minishell: fork");
		close(hd_prcs->pipe_fd[0]);
		close(hd_prcs->pipe_fd[1]);
		return (-1);
	}
	else if (hd_prcs->pid == 0)
	{
		close(hd_prcs->pipe_fd[0]);
		heredoc_child_process(hd_prcs->pipe_fd[1], delimiter, mini);
	}
	return (hd_prcs->pid);
}

//Main function for handling heredoc
int	handle_heredoc(const char *delimiter, t_shell *mini)
{
	t_process	hd_prcs;
	int			result;

	if (!delimiter || !mini)
	{
		ft_putstr_fd("Error: Invalid arguments to handle_hd \n" ,STDERR_FILENO);
		return (-1);
	}
	initialize_process(&hd_prcs);
	if (setup_heredoc_pipe(hd_prcs.pipe_fd) == -1)
		return (-1);
	hd_prcs.pid = fork_heredoc_process(&hd_prcs, delimiter, mini);
	if (hd_prcs.pid == -1)
		return (-1);
	result = handle_heredoc_parent(&hd_prcs, mini);
	return (result);
}
