/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 10:57:19 by frahenin          #+#    #+#             */
/*   Updated: 2025/02/05 18:57:04 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	exit_not_number(int sa_stdin, int sa_stdout, t_shell *shell,
		char **argv)
{
	if (sa_stdin != -1)
		close(sa_stdin);
	if (sa_stdout != -1)
		close(sa_stdout);
	write(STDOUT_FILENO, "exit\n", 5);
	ft_putstr_fd(argv[1], 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	ft_free_all(shell);
	g_global()->exit_status = 2;
	exit(g_global()->exit_status);
}

static int	check_number(char *str)
{
	int	i;

	i = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!(str[i] >= '0' && str[i] <= '9'))
		return (1);
	while ((str[i] >= '0' && str[i] <= '9') && str[i])
		i++;
	while ((str[i] == 32 || (str[i] >= 9 && str[i] <= 13)) && (str[i]))
		i++;
	if (str[i])
		return (1);
	return (0);
}

static __int128	ft_atoll(char *str)
{
	int			i;
	int			sign;
	__int128	res;

	sign = 1;
	i = 0;
	res = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while ((str[i] >= '0' && str[i] <= '9') && (str[i]))
	{
		res = (res * 10) + (str[i] - '0');
		i++;
	}
	return (res * sign);
}

static void	exit_number(int sa_stdin, int sa_stdout, t_shell *shell,
		char **argv)
{
	__int128	nbr;

	nbr = ft_atoll(argv[1]);
	if (nbr > LLONG_MAX || nbr < LLONG_MIN)
		exit_not_number(sa_stdin, sa_stdout, shell, argv);
	if (sa_stdin != -1)
		close(sa_stdin);
	if (sa_stdout != -1)
		close(sa_stdout);
	g_global()->exit_status = nbr % 256;
	if (g_global()->use_pipe == 0)
		write(STDOUT_FILENO, "exit\n", 5);
	ft_free_all(shell);
	exit(g_global()->exit_status);
}

int	ft_exit(t_shell *shell, char **argv, int sa_stdin, int sa_stdout)
{
	if (!argv[1])
	{
		if (sa_stdin != -1)
			close(sa_stdin);
		if (sa_stdout != -1)
			close(sa_stdout);
		ft_free_all(shell);
		if (g_global()->use_pipe == 0)
			write(STDOUT_FILENO, "exit\n", 5);
		exit(g_global()->exit_status);
	}
	if (check_number(argv[1]))
		exit_not_number(sa_stdin, sa_stdout, shell, argv);
	if (!check_number(argv[1]) && !argv[2])
		exit_number(sa_stdin, sa_stdout, shell, argv);
	else
	{
		if (g_global()->use_pipe == 0)
			write(STDOUT_FILENO, "exit\n", 5);
		ft_putstr_fd("exit: too many arguments\n", 2);
		g_global()->exit_status = 1;
		return (g_global()->exit_status);
	}
	return (g_global()->exit_status);
}
