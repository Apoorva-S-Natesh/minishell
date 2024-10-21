/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:51:27 by asomanah          #+#    #+#             */
/*   Updated: 2024/10/01 16:57:53 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
static void	init_env_array(t_envv_array *en_ar, t_env *env);

void	initialize_process(t_process *prcs)
{
	prcs->pid = -1;
	prcs->input_fd = -1;
	prcs->output_fd = -1;
	prcs->pipe_fd[0] = -1;
	prcs->pipe_fd[1] = -1;
	prcs->status = 0;
	prcs->exit_code = 0;
	prcs->signal = 0;
	prcs->cmd_path = NULL;
}

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
	free(prcs->cmd_path);
	prcs->cmd_path = NULL;
}

static void	init_env_array(t_envv_array *en_ar, t_env *env)
{
	en_ar->count = 0;
	en_ar->temp = env;
	en_ar->i = 0;
}
char	**create_env_array(t_env *env)
{
	t_envv_array	en_ar;

	init_env_array(&en_ar, env);
	while (en_ar.temp)
	{
		en_ar.count++;
		en_ar.temp = en_ar.temp->next;
	}
	en_ar.env_array = malloc(sizeof(char *) * (en_ar.count + 1));
	if (!en_ar.env_array)
		return (NULL);
	while (env)
	{
		en_ar.temp_str = ft_strjoin(env->key, "=");
		if (!en_ar.temp_str)
			return (NULL);
		en_ar.env_array[en_ar.i] = ft_strjoin(en_ar.temp_str, env->value);
		free(en_ar.temp_str);
		if (!en_ar.env_array[en_ar.i])
			return (NULL);
		env = env->next;
		en_ar.i++;
	}
	en_ar.env_array[en_ar.i] = NULL;
	return (en_ar.env_array);
}

static int	setup_pipes(int pipe_fd[2], t_command *cmd)
{
	if (cmd->next != NULL)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe");
			return (1);
		}
	}
	return (0);
}

static void	handle_child_process(int prev_pipe[2], int pipe_fd[2], t_command *cmd, t_shell *mini)
{
	if (prev_pipe[0] != -1)
	{
		dup2(prev_pipe[0], STDIN_FILENO);
		close(prev_pipe[0]);
		close(prev_pipe[1]);
	}
	if (cmd->next != NULL)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(mini->signal_pipe[0]);
}

static void	handle_parent_process(int prev_pipe[2], int pipe_fd[2], t_command *cmd)
{
	if (prev_pipe[0] != -1)
	{
		close(prev_pipe[0]);
		close(prev_pipe[1]);
	}
	if (cmd->next != NULL)
	{
		prev_pipe[0] = pipe_fd[0];
		prev_pipe[1] = pipe_fd[1];
	}
}
static void	wait_for_child(t_process *prcs, t_shell *mini)
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
		if (ret == -1)
		{
			if (errno == EINTR)
				continue ;
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

static void	exexcute_single_command(t_command *cmd, t_process *prcs, t_shell *mini, int prev_pipe[2])
{
	int pipe_fd[2];

	if (setup_pipes(pipe_fd, cmd) != 0)
	{
		mini->last_exit_status = 1;
		return ;
	}
	setup_redirs(cmd, prcs, &mini->redir_info, mini);
	if (is_builtin(cmd))
		handle_builtin(cmd, mini);
	else
	{
		prcs->pid = fork();
        if (prcs->pid == 0)
		{
			handle_child_process(prev_pipe, pipe_fd, cmd, mini);
			execute_command(cmd, prcs, mini);
			exit(mini->last_exit_status);
		}
		else if (prcs->pid < 0)
		{
			perror("fork");
			exit(1);
		}
		else
		{
			mini->foreground_pid = prcs->pid;
			handle_parent_process(prev_pipe, pipe_fd, cmd);
			if (cmd->next == NULL)
			{
				wait_for_child(prcs, mini);
				mini->foreground_pid = -1;
				handle_child_status(prcs, mini);
			}
		}
	}
	cleanup_redirections(prcs);
}

void execute(t_shell *mini)
{
	t_process prcs;
	t_command *cmd;
	int prev_pipe[2];

	prev_pipe[0] = -1;
	prev_pipe[1] = -1;
	mini->redir_info.tempin = dup(STDIN_FILENO);
	mini->redir_info.tempout = dup(STDOUT_FILENO);
	cmd = mini->commands;
	initialize_process(&prcs);
	while (cmd != NULL)
	{
		exexcute_single_command(cmd, &prcs, mini, prev_pipe);
		cmd = cmd->next;
	}
    while (wait(NULL) > 0);
    dup2(mini->redir_info.tempin, STDIN_FILENO);
    dup2(mini->redir_info.tempout, STDOUT_FILENO);
    close(mini->redir_info.tempin);
    close(mini->redir_info.tempout);
}

void	execute_command(t_command *cmd, t_process *prcs, t_shell *mini)
{
	char **env_array;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	env_array = create_env_array(mini->env);
	prcs->cmd_path = find_command(cmd->tokens[0], mini->env);
	printf("Debug : path is : %s\n", prcs->cmd_path);
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
	free_env_array(env_array);
	exit(1);
}
