/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschmidt <aschmidt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:41:01 by aschmidt          #+#    #+#             */
/*   Updated: 2024/09/23 15:29:13 by aschmidt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int ac, char **av, char **envv)
{
    t_shell mini;

    if (!check_args(ac, av))
        return (0);
    init_shell(&mini, envv);
    while (mini.running_status)
    {
        if (take_input(&mini))
            printf("hay input!: %s/n", mini.input);
            //tokenize
        //execute
    }
   free_all(&mini);
	return (0);
}
