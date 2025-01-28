/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 10:57:19 by frahenin          #+#    #+#             */
/*   Updated: 2025/01/28 17:50:11 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_bool	ft_is_number(char *s)
{
	if (!s)
		return (FALSE);
	while (*s)
	{
		if (!ft_isdigit(*s))
			return (FALSE);
		s++;
	}
	return (TRUE);
}

int	ft_exit(t_shell *shell, char **argv, int stdin, int stdout)
{
	if (!argv[1])
	{
		close(stdin);
		close(stdout);
		ft_free_all(shell);
		write(STDOUT_FILENO, "exit\n", 5);
		exit(g_global()->exit_status);
	}
	if (!ft_is_number(argv[1]))
	{
		close(stdin);
		close(stdout);
		write(STDOUT_FILENO, "exit\n", 5);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		ft_free_all(shell);
		g_global()->exit_status = 2;
		exit (g_global()->exit_status);
	}
	if (ft_is_number(argv[1]) && !argv[2])
	{
		close(stdin);
		close(stdout);
		g_global()->exit_status =  ft_atoi(argv[1]);
		write(STDOUT_FILENO, "exit\n", 5);
		ft_free_all(shell);
		exit(g_global()->exit_status);
	}
	else
	{
		write(STDOUT_FILENO, "exit\n", 5);
		ft_putstr_fd("exit: too many arguments\n", 2);
		g_global()->exit_status = 1;
		return (g_global()->exit_status);
	}
	return (0);
}
