#include "minishell.h"
char	is_builtin(t_command *cmd)
{
	if ((strcmp(cmd->tokens[0], "echo")) || 
		(strcmp(cmd->tokens[0], "cd")) ||
		(strcmp(cmd->tokens[0], "pwd")) ||
		(strcmp(cmd->tokens[0], "export")) ||
		(strcmp(cmd->tokens[0], "unset")) || 
		(strcmp(cmd->tokens[0], "env")) ||
		(strcmp(cmd->tokens[0], "exit")))
			return (1);
	return (0);
}

void	handle_builtin(t_command *cmd)
{
	if (strcmp(cmd->tokens[0], "echo") == 0) //change to libft strcmp (now string.h strcmp)
		builtin_echo(&cmd->tokens);
	if (strcmp(cmd->tokens[0], "cd") == 0)
		builtin_cd(&cmd->tokens);
	if (strcmp(cmd->tokens[0], "pwd") == 0)
		builtin_pwd(&cmd->tokens);
	if (strcmp(cmd->tokens[0], "export") == 0)
		builtin_export(&cmd->tokens);
	if (strcmp(cmd->tokens[0], "unset") == 0)
		builtin_unset(&cmd->tokens);
	if (strcmp(cmd->tokens[0], "env") == 0)
		builtin_env(&cmd->tokens);
	if (strcmp(cmd->tokens[0], "exit") == 0)
		builtin_exit(&cmd->tokens);
}
void	builtin_echo(char **args)
{

}
