/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:51:27 by asomanah          #+#    #+#             */
/*   Updated: 2024/10/31 17:53:27 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	execute(t_shell *mini)
{
	t_process	prcs;
	t_command	*cmd;
	t_pipe_info	pipe_info;

	mini->redir_info.tempin = dup(STDIN_FILENO);
	mini->redir_info.tempout = dup(STDOUT_FILENO);
	cmd = mini->commands;
	initialize_process(&prcs);
	initialize_pipe_info(&pipe_info);
	while (cmd != NULL)
	{
		execute_single_command(cmd, &prcs, mini, &pipe_info);
		cleanup_redirections(&prcs);
		cmd = cmd->next;
	}
	finish_execution(mini);
}

void	execute_single_command(t_command *cmd, t_process *prcs, \
t_shell *mini, t_pipe_info *pipe_info)
{
	int	redir_result;

	redir_result = setup_redirs(cmd, prcs, &mini->redir_info, mini);
	if (redir_result <= 0)
	{
		handle_redir_error(redir_result, mini);
		return ;
	}
	if (cmd->next || (!cmd->tokens || !cmd->tokens[0]))
	{
		if (create_pipe(pipe_info->pipe_fd) == -1)
			return ;
	}
	if (is_builtin(cmd))
		execute_builtin(cmd, mini, pipe_info);
	else if (cmd->tokens && cmd->tokens[0])
		execute_non_builtin(cmd, prcs, mini, pipe_info);
	cleanup_pipes(cmd, pipe_info);
}

void	execute_builtin(t_command *cmd, t_shell *mini, t_pipe_info *pipe_info)
{
	int	stdout_copy;

	stdout_copy = dup(STDOUT_FILENO);
	if (cmd->next)
	{
		// Redirect stdout to the write end of the pipe
		dup2(pipe_info->pipe_fd[1], STDOUT_FILENO);
		close(pipe_info->pipe_fd[1]);
	}
	handle_builtin(cmd, mini);
	// Restore original stdout
	dup2(stdout_copy, STDOUT_FILENO);
	close(stdout_copy);
}

void	execute_non_builtin(t_command *cmd, t_process *prcs, \
t_shell *mini, t_pipe_info *pipe_info)
{
	t_exec_info	exec_info;

	exec_info.cmd = cmd;
	exec_info.prcs = prcs;
	exec_info.mini = mini;
	exec_info.pipe_info = *pipe_info;
	prcs->pid = fork();
	if (prcs->pid == 0)
		handle_child_process(&exec_info);
	else if (prcs->pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else
		handle_parent_process(&exec_info);
}

void	execute_command(t_command *cmd, t_process *prcs, t_shell *mini)
{
	char	**env_array;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	env_array = create_env_array(mini->env);
	prcs->cmd_path = find_command(cmd->tokens[0], mini->env);
	if (!prcs->cmd_path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putstr_fd(cmd->tokens[0], 2);
		ft_putstr_fd("\n", 2);
		mini->last_exit_status = 127;
		free_env_array(env_array);
		exit(127);
	}
	execve(prcs->cmd_path, cmd->tokens, env_array);
	perror("execve");
	free(prcs->cmd_path);
	free_env_array(env_array);
	exit(1);
}

void	handle_parent_process(t_exec_info *exec_info)
{
	if (exec_info->pipe_info.prev_pipe[0] != -1)
	{
		close(exec_info->pipe_info.prev_pipe[0]);
		close(exec_info->pipe_info.prev_pipe[1]);
	}
	if (exec_info->cmd->next != NULL)
	{// Save the read end of the current pipe for the next command
		close(exec_info->pipe_info.pipe_fd[1]);
		exec_info->pipe_info.prev_pipe[0] = exec_info->pipe_info.pipe_fd[0];
		exec_info->pipe_info.prev_pipe[1] = exec_info->pipe_info.pipe_fd[1];
	}
	else //close the pipe file descriptors if it's the last command.
	{
		close(exec_info->pipe_info.pipe_fd[0]);
		close(exec_info->pipe_info.pipe_fd[1]);
	}
	if (exec_info->cmd->next == NULL)
	{
		wait_for_child(exec_info->prcs, exec_info->mini);
		handle_child_status(exec_info->prcs, exec_info->mini);
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
}
