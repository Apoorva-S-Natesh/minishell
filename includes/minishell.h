/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 11:19:35 by aschmidt          #+#    #+#             */
/*   Updated: 2024/10/01 16:54:04 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

# include "../libft/src/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <signal.h>
# include <errno.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>

# ifndef PATH_MAX
# 	define PATH_MAX 4096
# endif

# ifndef HEREDOC_EOF
#	define  HEREDOC_EOF "minishell: warning: here-document \
delimited by end-of-file\n"
# endif

typedef struct s_env			t_env;
typedef struct s_redirection	t_redirection;
typedef struct s_token			t_token;
typedef struct s_command		t_command;

#define SUCCESS 0

typedef struct s_env
{
	char		*key;               // (e.g., "PATH")
	char		*value;             // Path value (e.g., "/usr/bin:/bin")
	t_env		*next;
}	t_env;

typedef struct s_redirection
{
	char			*input_file; //input or output
	char			*output_file;
	int				type; // (1 for <) (2 for >) (3 for >>) (4 for <<)
	t_redirection	*next;
}	t_redirection;

typedef struct s_redir_info
{
	int	tempin;
	int	tempout;
} t_redir_info;


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
	char			*value; //for a command can have absolute or relative path
	e_token_type	type;
	t_token			*next;
}	t_token;

typedef struct s_command
{
	char			**tokens; //array of nodes of tokens
	int				type; //1 for builtin - 0 for path
	int				priority; // 1 for << , increase from left to right, 0 if quotes failed
	t_redirection	*redirection; // if there are < or > or >> inside the command. Pointer to the list
	t_command		*prev;
	t_command		*next;
}	t_command;

typedef struct s_shell
{
	t_command	*commands; //pointer to the head of commands list
	char		*input; //store the original input (?)
	t_env		*env;
	int			running_status;
	int			signal_received;
	char		cwd[1024];
	int			last_exit_status; // Track the last exit status of executed commands
	int			expand_heredoc; //struct to control variable expansion in heredocs
}	t_shell;

typedef struct s_process
{
    pid_t 	pid;               // Process ID of the forked child
    int 	input_fd;            // File descriptor for input redirection
    int 	output_fd;           // File descriptor for output redirection
    int 	pipe_fd[2];          // Pipe file descriptors for input/output communication
    //struct s_process *next;  // Next process in a pipeline
	int		status;
	int		exit_code;
	int		signal;
	char	*cmd_path; // Full path of the command to be excuted
} t_process;

//INIT SHELL
void    init_shell(t_shell *mini, char **envv);
void    set_envv(char **envv);

//INPUT
int		check_args(int ac, char **av);
int	takeInput(char* str);

//SIGNAL
void	handle_sigint(int sig);
void	handle_sigint_heredoc(int signum);

// EXECUTE
char	**split_path(char *path);
char	*check_cmd_in_path(char *cmd, char **paths);
char	*find_command(char *cmd, char **env);

//heredoc
void	heredoc_read_loop(int fd, const char *delimiter, t_shell *mini);
void	heredoc_child_process(int wr_fd, const char *delimiter, t_shell *mini);
int		handle_heredoc(const char *delimiter, t_shell *mini);

// BUILTINS
char	is_builtin(t_command *cmd);
void	handle_builtin(t_command *cmd, t_shell *mini);
void	builtin_cd(char **tokens, t_shell *mini, int size);
char	*ft_getcwd(t_shell *shell);
int		builtin_echo(char **tokens, t_shell *mini, int size);
void	handle_env_expansion(char *arg, t_env *env); //Delete function if already present 
void	builtin_env(char **tokens, t_shell *mini, int size);
void	builtin_exit(char **tokens, t_shell *mini, int size);
int		is_valid_identifier(const char *str);
void	builtin_export(char **tokens, t_shell *mini);
void	builtin_pwd(t_shell *mini);
void	builtin_unset(t_shell *mini, char **args);

// UTILS
char	*ft_getenv(const char *name, t_env *env);
void	ft_putstr_fd(char *s, int fd);

#endif