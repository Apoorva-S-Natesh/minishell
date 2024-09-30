#include "minishell.h"

int main(int argc, char **argv, char **env) {
    // Initialize shell environment
    t_shell shell;
    shell.env = NULL;
    shell.running_status = 0;
    shell.signal_received = 0;

    // Set up a test environment variable for HOME
    t_env home_env;
    home_env.key = strdup("HOME");
    home_env.value = strdup("/home/user");
    home_env.next = NULL;
    shell.env = &home_env;

    // Create a command to test 'cd' with no arguments (should go to HOME)
    t_command command;
    char *tokens1[] = {"cd", NULL};
    command.tokens = tokens1;
    command.type = 1;
    command.priority = 0;
    command.last_exit_status = 0;
    command.redirection = NULL;
    command.prev = NULL;
    command.next = NULL;

    // Execute the command
    printf("Executing 'cd' with no arguments...\n");
    execute_command(&shell, &command);

    // Print the current working directory
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    } else {
        perror("getcwd");
    }

    // Create a command to test 'cd /'
    char *tokens2[] = {"cd", "/", NULL};
    command.tokens = tokens2;

    // Execute the command
    printf("Executing 'cd /'...\n");
    execute_command(&shell, &command);

    // Print the current working directory
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    } else {
        perror("getcwd");
    }

    // Create a command to test 'cd -'
    char *tokens3[] = {"cd", "-", NULL};
    command.tokens = tokens3;

    // Execute the command
    printf("Executing 'cd -'...\n");
    execute_command(&shell, &command);

    // Print the current working directory
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    } else {
        perror("getcwd");
    }

    return 0;
}
