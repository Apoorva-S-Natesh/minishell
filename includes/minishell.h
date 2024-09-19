/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschmidt <aschmidt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 11:19:35 by aschmidt          #+#    #+#             */
/*   Updated: 2024/09/19 14:55:21 by aschmidt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <dirent.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_env			t_env;
typedef struct s_redirection	t_redirection;
typedef struct s_token			t_token;
typedef struct s_command		t_command;

typedef struct s_env
{
	char		*key;               // (e.g., "PATH")
	char		*value;             // Path value (e.g., "/usr/bin:/bin")
	t_env		*next;
}	t_env;

typedef struct s_redirection
{
	char			*filename; //input or output
	int				type; // (1 for <) (2 for >) (3 for >>)
	t_redirection	*next;
}	t_redirection;

typedef	enum token_type
{
	WORD,
	FILE_NAME, //if < or > the word at RIGHT should be filename
	CMD,
	//ARG,
	RED_IN,
	RED_OUT,
	APPEND,
	HEREDOC,
	PIPE,
	ENV,
	SINGLE_Q,
	DOUBLE_Q
}	e_token_type;

typedef struct s_token
{
	char			*value;
	e_token_type	type;
	t_token			*next;
}	t_token;

typedef struct s_command
{
	char			**tokens; //array of nodes of tokes
	int				type; //1 for builtin - 0 for path
	int				priority; // 1 for << , increase from left to right, 0 if quotes failed
	int				last_exit_status;
	t_redirection	*redirection; // if there are < or > or >> inside the command. Pointer to the list
	t_command		*prev;
	t_command		*next;
}	t_command;

typedef struct s_shell
{
	t_command	*commands; //pointer to the head of commands list
	char		*input; //store the original input (?)
	t_env		*env;
	int			status;
	int			signal_received;
}	t_shell;

typedef struct s_process
{
    pid_t pid;               // Process ID of the forked child
    int input_fd;            // File descriptor for input redirection
    int output_fd;           // File descriptor for output redirection
    int pipe_fd[2];          // Pipe file descriptors for input/output communication
    struct s_process *next;  // Next process in a pipeline
} t_process;

int	takeInput(char* str);

#endif
