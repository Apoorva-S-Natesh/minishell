/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 11:19:35 by aschmidt          #+#    #+#             */
/*   Updated: 2024/11/15 00:54:28 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/src/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <stddef.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <signal.h>
# include <errno.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/select.h>
# include <sys/time.h>
# include <termios.h>
# include <sys/stat.h>

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

# ifndef HEREDOC_EOF
#  define HEREDOC_EOF "minishell: warning: here-document \
delimited by end-of-file\n"
# endif

typedef struct s_env			t_env;
typedef struct s_redirection	t_redirection;
typedef struct s_token			t_token;
typedef struct s_command		t_command;

# define SUCCESS 0

typedef struct s_env
{
	char	*key;
	char	*value;
	t_env	*next;
}	t_env;

typedef struct s_redirection
{
	char			*input_file;
	char			*output_file;
	int				type;
	char			*heredoc_delimiter;
	t_redirection	*next;
}	t_redirection;

typedef struct s_redir_info
{
	int	tempin;
	int	tempout;
}	t_redir_info;

typedef enum token_type
{
	WORD,
	PIPE,
	RED_IN,
	RED_OUT,
	APPEND,
	HEREDOC,
	FILE_NAME,
	CMD,
	ENV,
	SINGLE_Q,
	DOUBLE_Q
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	int				quote;
	int				concat_flag;
	t_token			*next;
}	t_token;

typedef struct s_command
{
	char			**tokens;
	int				priority;
	t_redirection	*redirection;
	t_command		*next;
}	t_command;

typedef struct s_shell
{
	t_command		*commands;
	char			*input;
	t_env			*env;
	t_token			*token;
	int				running_status;
	int				signal_received;
	char			cwd[1024];
	int				last_exit_status;
	int				expand_heredoc;
	int				signal_pipe[2];
	pid_t			foreground_pid;
	t_redir_info	redir_info;
}	t_shell;

typedef struct s_process
{
	pid_t	pid;
	int		input_fd;
	int		output_fd;
	int		pipe_fd[2];
	int		status;
	int		exit_code;
	int		signal;
	char	*cmd_path;
}	t_process;

typedef struct s_envv_array
{
	int		count;
	t_env	*temp;
	char	**env_array;
	int		i;
	char	*temp_str;
}	t_envv_array;

typedef struct s_pipe_info
{
	int	prev_pipe[2];
	int	pipe_fd[2];
}	t_pipe_info;

typedef struct s_exec_info
{
	t_command	*cmd;
	t_process	*prcs;
	t_shell		*mini;
	t_pipe_info	pipe_info;
}	t_exec_info;

typedef struct s_expand_info
{
	char	*result;
	size_t	size;
	size_t	len;
	char	*tkn_ptr;
	t_shell	*mini;
}	t_expand_info;

// Struct to manage state during tokenization
typedef struct s_tokenizer 
{
	char	*input;
	char	buffer[256];
	int		buffer_index;
	int		in_quotes;
	char	quote_char;
}	t_tokenizer;

//INIT SHELL
void			init_shell(t_shell *mini, char **envv);
void			set_envv(t_shell *mini, char **envv);
void			execute_minishell(t_shell *mini, t_token *tokens);

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
int				validate_tokens(t_token *tokens);
void			handle_word(t_shell *mini, int *i, t_token **tokens);
void			set_redi_and_pipes(t_shell *mini, int *i, t_token **tokens);
int				handle_quotes(t_shell *mini, int *i, t_token **tokens);
t_token_type	classify_token(char *token_value);
void			append_token(t_token **tokens, char *value, t_token_type type, \
				int quote_type);
void			set_concat_flag(char *input, int i, t_token *last_token);
void			append_or_concat_token(t_token **tokens, char *value, \
				int type, int quote_type);
t_token			*get_last_token(t_token *tokens);
void			handle_quote(t_tokenizer *tokenizer, int *i);
void			handle_equals(t_tokenizer *tokenizer, int *i);
int				should_break(char c, int in_quotes);

//EXPAND TOKENS
void			expand_tokens(t_token **tokens, t_shell *mini);
char			*expand_value(char	*token, t_shell *mini);
char			*extract_env(char **ptr, t_shell *mini);
char			*get_env_value(const char *name, t_env *env_list);
char			*remove_quotes(char *str);
char			*extract_quoted_content(char *input, int *i, int quote_type, \
				int buffer_index);
int				resize_result(t_expand_info *info, size_t needed);
int				append_var_value(t_expand_info *info, char *value);
int				handle_dollar_sign(t_expand_info *info);
int				append_char(t_expand_info *info, char c);
void			initialize_info(t_expand_info *info, char *token, \
				t_shell *mini);

//CREATE COMMAND
t_command		*group_tokens_to_cmd(t_token *tokens);
t_command		*get_command(t_command *current, t_command **head);
t_command		*create_new_command(void);
void			set_cmd_priorities(t_command *cmd_head);
void			append_command(t_command **head, t_command *new_command);
void			process_token(t_command **current_cmd, t_token **current_tkn);
void			add_tkn_to_cmd(t_command *cmd, t_token *token);
void			handle_redirection(t_command **cmd, t_token **current_token);
void			add_redi_to_cmd(t_command *cmd, t_token *redir_token, \
				char *filename);
void			set_redi_type(t_redirection *redir, t_token *redir_token, \
				char *filename);
void			append_redi(t_command *cmd, t_redirection *redir);

//SIGNAL
void			handle_sigint(int sig);
void			setup_sig_handling(t_shell *mini);
void			setup_heredoc_signals(void);
void			restore_main_signals(void);
void			setup_child_signals(void);
void			cleanup_signal_pipe(t_shell *mini);

// EXECUTE
char			**split_path(char *path);
char			*check_cmd_in_path(char *cmd, char **paths);
char			**create_env_array(t_env *env);
char			*find_command(char *cmd, t_env *variable);
void			initialize_process(t_process *prcs);
int				setup_redirs(t_command *cmd, t_process *prcs, \
				t_redir_info *re, t_shell *mini);
void			execute(t_shell *mini);
void			cleanup_redirections(t_process *prcs);
void			handle_child_status(t_process *prcs, t_shell *mini);
void			initialize_process(t_process *prcs);
void			execute(t_shell *mini);
int				execute_command(t_command *cmd, t_process *prcs, t_shell *mini);
void			handle_child_status(t_process *prcs, t_shell *mini);
int				handle_child_process(t_exec_info *exec_info);
int				setup_pipes(int pipe_fd[2], t_command *cmd);
void			init_env_array(t_envv_array *en_ar, t_env *env);
void			execute_single_command(t_command *cmd, t_process *prcs, \
				t_shell *mini, t_pipe_info *pipe_info);
void			finish_execution(t_shell *mini);
void			execute_builtin(t_command *cmd, t_shell *mini, \
				t_pipe_info *pipe_info);
void			execute_non_builtin(t_command *cmd, t_process *prcs, \
t_shell *mini, t_pipe_info *pipe_info);
void			handle_redir_error(int redir_result, t_shell *mini);
void			handle_parent_process(t_exec_info *exec_info);
void			wait_for_child(t_process *prcs);
int				check_file_status(const char *cmd_path);
void			handle_command_error(const char *cmd_path, \
				const char *cmd_name, int status);
int				execute_command_helper(t_command *cmd, char *cmd_path, \
				char **env_array);
void			handle_exection_pipes(t_pipe_info *pipe_info, t_command *cmd);

//pipes
void			initialize_pipe_info(t_pipe_info *pipe_info);
void			cleanup_pipes(t_command *cmd, t_pipe_info *pipe_info);
int				create_pipe(int pipe_fd[2]);

//redirections
void			print_err_msg(const char *filename, const char *message);
int				setup_input_redir(t_process *prcs, t_redirection *redir, \
				t_shell *mini);
int				setup_output_redir(t_process *prcs, t_redirection *redir, \
				t_shell *mini);
void			cleanup_redirections(t_process *prcs);
void			handle_redir_error(int redir_result, t_shell *mini);

//heredoc
void			heredoc_read_loop(int fd, const char *delimiter, t_shell *mini);
void			heredoc_child_process(int wr_fd, const char *delimiter, \
				t_shell *mini);
int				handle_heredoc(const char *delimiter, t_shell *mini);

// BUILTINS
char			is_builtin(t_command *cmd);
void			handle_builtin(t_command *cmd, t_shell *mini, int in_pipeline);
void			builtin_cd(char **tokens, t_shell *mini, int size);
char			*ft_getcwd(t_shell *mini);
int				builtin_echo(char **tokens, t_shell *mini, int size);
void			builtin_env(t_shell *mini, int size);
void			builtin_exit(char **tokens, t_shell *mini, int size);
int				is_valid_identifier(const char *str);
void			builtin_export(char **tokens, t_shell *mini, int in_pipeline);
void			print_exported_vars(t_env *env);
int				set_env_variable(t_shell *mini, const char *key, \
				const char *value);
void			builtin_pwd(t_shell *mini);
void			builtin_unset(t_shell *mini, char **args);

// UTILS
void			initialize_pipe_info(t_pipe_info *pipe_info);
void			free_env_array(char **env_array);
int				many_args(int num_args);

//PRINT DEBUG FUNCTIONS
void			print_redirections(t_redirection *redir);
void			print_command_tokens(t_command *cmd);
void			print_commands(t_command *commands);
void			print_tokens(t_token *tokens);

#endif
