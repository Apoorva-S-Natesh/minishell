/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschmidt <aschmidt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 14:19:49 by aschmidt          #+#    #+#             */
/*   Updated: 2024/09/24 10:03:24 by aschmidt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_args(int ac, char **av)
{
	if (ac != 1 || av[1])
    {
        printf ("Program doesn't take arguments!");
        return (0);
    }
	return (1);
}

int is_blank(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (!ft_isspace(str[i]))
            return (0);
        i++;
    }
    return (1);
}

int take_input(t_shell *mini)
{

    mini->input = readline("\n>>> ");
    if (ft_strlen(mini->input) != 0)
    {
        if (is_blank(mini->input))
        {
            free(mini->input);
            return (0);
        }
        add_history(mini->input);
        return (1);
    }
    else
        return (0);
}

//check tty
