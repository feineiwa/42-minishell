/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:12:38 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/13 16:07:36 by nrasamim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int  is_number(char *str)
{
    while (*str)
    {
        if (!ft_isdigit(*str))
            return (0);
        str++;
    }
    return (1);
}

static void free_all(t_shell **shell)
{
    rl_clear_history();
    ft_free_cmd(&(*shell)->cmd);
    ft_free_env(&(*shell)->envp);
}

int ft_exit(t_shell *shell, char **args)
{
    int exit_code;

    if (!args[1])
    {
        free_all(&shell);
        exit(0);
    }
    else if (args[2])
    {
        perror("exit: too many arguments");
        return (1);
    }
    else if (is_number(args[1]))
    {
        exit_code = ft_atoi(args[1]);
        free_all(&shell);
        exit(exit_code);
    }
    else
    {
        perror("exit: numeric argument required");
        return (1);
    }
}
