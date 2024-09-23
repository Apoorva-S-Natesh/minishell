/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschmidt <aschmidt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:41:01 by aschmidt          #+#    #+#             */
/*   Updated: 2024/09/23 13:24:20 by aschmidt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
/*
void printDir()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDir: %s", cwd);
}*/

void    init_shell(t_shell *mini, char **envv)
{
    mini->running_status = 1;
    printf("the status is %d\n", mini->running_status);
    set_envv(mini, envv);
}

void    set_envv(t_shell *mini, char **envv)
{
    int i;
    t_env   *head;
    char **split_envv;

    i = 0;
    head = NULL;
    while (envv[i] != NULL)
    {
        split_envv = ft_split(envv[i], '=');
        append_node(&head, split_envv[0], split_envv[1]);
        i++;
    }
    mini->env = head;
    print_list(mini->env);
}
int	main(int ac, char **av, char **envv)
{
    t_shell mini;


    if (!check_args(ac, av))
        return (0);
    init_shell(&mini, envv);
    //printDir();
    /*loop
    while (mini.status)
    {
        intialize_shell();
        // read input
        //tokenize
        //execute
    }
    */

	return (0);
}
