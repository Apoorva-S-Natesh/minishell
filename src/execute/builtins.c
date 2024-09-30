#include "minishell.h"
char	is_builtin(t_command *cmd)
{
	if ((strcmp(cmd->tokens[0], "echo") == 0) || 
		(strcmp(cmd->tokens[0], "cd") == 0) ||
		(strcmp(cmd->tokens[0], "pwd") == 0) ||
		(strcmp(cmd->tokens[0], "export") == 0) ||
		(strcmp(cmd->tokens[0], "unset") == 0) || 
		(strcmp(cmd->tokens[0], "env") == 0) ||
		(strcmp(cmd->tokens[0], "exit") == 0))
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
		builtin_pwd();
	if (strcmp(cmd->tokens[0], "export") == 0)
		builtin_export(&cmd->tokens);
	if (strcmp(cmd->tokens[0], "unset") == 0)
		builtin_unset(&cmd->tokens);
	if (strcmp(cmd->tokens[0], "env") == 0)
		builtin_env();
	if (strcmp(cmd->tokens[0], "exit") == 0)
		builtin_exit();
}
void	builtin_echo(char **args) //what happens if the input has a space at the end? is it considered in the tokens?
{
	int	n_flag;
	int	i;

	i = 1;
	n_flag = 0;
	if(args[1] && strcmp(args[1], "-n") == 0)
	{
		n_flag = 1;
		args++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (!n_flag)
		printf("\n");
}

void	builtin_cd(char **args)
{
	if (args[1] == NULL)
		printf(stderr, "cd: expected argument\n");
	else
	{
		if (chdir(args[1]) != 0)
			perror("cd");
	}
}

void	builtin_pwd()
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("cwd");
}
void	builtin_export(char **args) // How is it stored and remembered later in the entire life of the shell?
{
	char	*key;
	char	*value;

	if (args[1] == NULL)
		printf(stderr, "Export: expected aargument\n");
	else
	{
		
	}
}

void	builtin_exit()
{
	exit (0);
}
