/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 10:57:19 by frahenin          #+#    #+#             */
/*   Updated: 2025/01/27 14:02:56 by nrasamim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_bool	ft_is_number(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (FALSE);
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (FALSE);
		i++;
	}
	return (TRUE);
}

int	ft_exit(char **argv, int stdin, int stdout)
{
	if (ft_is_number(argv[1]))
	{
		close(stdin);
		close(stdout);
		g_global()->exit_status = ft_atoi(argv[1]);
		exit(g_global()->exit_status);
	}
	else if (!argv[1])
	{
		close(stdin);
		close(stdout);
		exit(g_global()->exit_status % 256);
	}
	else if (argv[2])
	{
		close(stdin);
		close(stdout);
		write(STDERR_FILENO, "exit: too many arguments", 25);
		exit(1);
	}
	else
	{
		close(stdin);
		close(stdout);
		write(STDERR_FILENO, "numeric argument required\n", 27);
		exit(2);
	}
}
