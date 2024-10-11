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
	char            *heredoc_delimiter; // If type == 4, store the heredoc delimiter here
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
	PIPE,
	RED_IN,
	RED_OUT,
	APPEND,
	HEREDOC,
	FILE_NAME, //if < or > the word at RIGHT should be filename
	CMD,
	//ARG,
	ENV,
	SINGLE_Q,
	DOUBLE_Q
}	e_token_type;

typedef struct s_token
{
	char			*value; //for a command can have absolute or relative path
	e_token_type	type;
	int				quote; // 0 = no quote, 1 = single quote, 2 = double quote
	t_token			*next;
}	t_token;

typedef struct s_command
{
	char			**tokens; //array of nodes of tokes
//	int				type; //1 for builtin - 0 for path
	int				priority; // 1 for << , increase from left to right, 0 if quotes failed
	t_redirection	*redirection; // if there are < or > or >> inside the command. Pointer to the list
//	t_command		*prev;
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
void			init_shell(t_shell *mini, char **envv);
void			set_envv(t_shell *mini, char **envv);

//INPUT
int				check_args(int ac, char **av);
int				take_input(t_shell *mini);

//ENVV LIST
void			append_node(t_env **head, char *key, char *value);
t_env			*new_env(char *key, char *value);
void			free_list(t_env *head);
void			print_list(t_env *head);

//FREE
void			ft_free(char **arr);
void			free_all(t_shell *mini);
void			free_tokens(t_token *tokens);
void			free_redirections(t_redirection *redirection);
void			free_command(t_command *command);

//CREATE TOKEN
t_token			*tokenize(t_shell *mini);
void			handle_word(t_shell *mini, int *i, t_token **tokens);
void			set_redi_and_pipes(t_shell *mini, int *i, t_token **tokens);
int				handle_quotes(t_shell *mini, int *i, t_token **tokens);
e_token_type	classify_token(char *token_value);
void			append_token(t_token **tokens, char *value, e_token_type type, int quote_type);

//EXPAND TOKENS
void			expand_tokens(t_token *tokens, t_env *env_list);
char			*expand_value(char	*token, t_env *env_list);
char			*extract_env(char **ptr, t_env *env_list);
char			*get_env_value(const char *name, t_env *env_list);
int 			process_operators(t_token **tokens, char **start, char **end);
char			*remove_quotes(char *str);
char			*extract_quoted_content(char *input, int *i, int quote_type, int buffer_index);

//CREATE COMMAND
t_command		*group_tokens_to_cmd(t_token *tokens);
t_command		*get_command(t_command *current, t_command **head);
t_command		*create_new_command(void);
void			set_cmd_priorities(t_command *cmd_head);
void			append_command(t_command **head, t_command *new_command);
void			process_token(t_command **current_cmd, t_token **current_tkn);
void			add_tkn_to_cmd(t_command *cmd, t_token *token);
void			handle_redirection(t_command **cmd, t_token **current_token);
void			add_redi_to_cmd(t_command *cmd, t_token *redir_token, char *filename);
void			set_redi_type(t_redirection *redir, t_token *redir_token, char *filename);
void			append_redi(t_command *cmd, t_redirection *redir);


//SIGNAL
void			handle_sigint(int sig);
void			handle_sigint_heredoc(int signum);

// EXECUTE
char			**split_path(char *path);
char			*check_cmd_in_path(char *cmd, char **paths);
char			**create_env_array(t_env *env);
char			*find_command(char *cmd, t_env *variable);
void			initialize_process(t_process *prcs);
void			print_redir_err(const char *filename, const char *message);
void			setup_redirs(t_command *cmd, t_process *prcs, t_redir_info *re, t_shell *mini);
void			execute(t_shell *mini);
void			cleanup_redirections(t_process *prcs);
void			execute_command(t_command *cmd, t_process *prcs, t_shell *mini);
void			handle_child_status(t_process *prcs, t_shell *mini);
void			initialize_process(t_process *prcs);
void			print_redir_err(const char *filename, const char *message);
void			setup_redirs(t_command *cmd, t_process *prcs, t_redir_info *re, t_shell *mini);
void			execute(t_shell *mini);
void			cleanup_redirections(t_process *prcs);
void			execute_command(t_command *cmd, t_process *prcs, t_shell *mini);
void			handle_child_status(t_process *prcs, t_shell *mini);

//heredoc
void			heredoc_read_loop(int fd, const char *delimiter, t_shell *mini);
void			heredoc_child_process(int wr_fd, const char *delimiter, t_shell *mini);
int				handle_heredoc(const char *delimiter, t_shell *mini);

// BUILTINS
char			is_builtin(t_command *cmd);
void			handle_builtin(t_command *cmd, t_shell *mini);
void			builtin_cd(char **tokens, t_shell *mini, int size);
char			*ft_getcwd(t_shell *mini);
int				builtin_echo(char **tokens, t_shell *mini, int size);
void			builtin_env(t_shell *mini, int size);
void			builtin_exit(char **tokens, t_shell *mini, int size);
int				is_valid_identifier(const char *str);
void			builtin_export(char **tokens, t_shell *mini);
void			builtin_pwd(t_shell *mini);
void			builtin_unset(t_shell *mini, char **args);

// UTILS
void			free_env_array(char **env_array);

//////////////////////////////////Delete this after dicsussion
//Expand variable in handle heredoc (write_heredoc_line)
char 			*expand_variables(char *line, t_shell *mini);
char			*ft_strtok(char *str, const char *delim); // Delete this or rewrite (for finding path in exec_path.c)

#endif
