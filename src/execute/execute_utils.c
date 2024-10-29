/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:25:03 by asomanah          #+#    #+#             */
/*   Updated: 2024/10/29 14:25:08 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Function to check if cd has too many arguments
int	many_args(int num_args)
{
	if (num_args > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	return (0);
}

// Initializes the process struct
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

//initializes the struct
void	init_env_array(t_envv_array *en_ar, t_env *env)
{
	en_ar->count = 0;
	en_ar->temp = env;
	en_ar->i = 0;
}

//Array of env variables
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

int	setup_pipes(int pipe_fd[2], t_command *cmd)
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