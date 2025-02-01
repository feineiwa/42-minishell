/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 10:57:19 by frahenin          #+#    #+#             */
/*   Updated: 2025/02/01 12:47:02 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_bool	ft_is_number(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (FALSE);
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (!s[i])
		return (FALSE);
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (FALSE);
		i++;
	}
	return (TRUE);
}

static void	exit_not_number(int stdin, int stdout, t_shell *shell, char **argv)
{
	if (stdin != -1)
		close(stdin);
	if (stdout != -1)
		close(stdout);
	write(STDOUT_FILENO, "exit\n", 5);
	ft_putstr_fd(argv[1], 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	ft_free_all(shell);
	g_global()->exit_status = 2;
	exit(g_global()->exit_status);
}

long	ft_atol(char *str)
{
	int		i;
	int		sign;
	long	res;

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
	while ((str[i] >= '0' && str[i] <= '9') && str[i])
	{
		res = (res * 10) + (str[i] - '0');
		i++;
	}
	return (res * sign);
}

static void	exit_number(int stdin, int stdout, t_shell *shell, char **argv)
{
	long	nbr;

	nbr = ft_atol(argv[1]);
	if (nbr > INT_MAX || nbr < INT_MIN)
		exit_not_number(stdin, stdout, shell, argv);
	if (stdin != -1)
		close(stdin);
	if (stdout != -1)
		close(stdout);
	g_global()->exit_status = nbr % 256;
	write(STDOUT_FILENO, "exit\n", 5);
	ft_free_all(shell);
	exit(g_global()->exit_status);
}

int	ft_exit(t_shell *shell, char **argv, int stdin, int stdout)
{
	if (!argv[1])
	{
		if (stdin != -1)
			close(stdin);
		if (stdout != -1)
			close(stdout);
		ft_free_all(shell);
		write(STDOUT_FILENO, "exit\n", 5);
		exit(g_global()->exit_status);
	}
	if (!ft_is_number(argv[1]))
		exit_not_number(stdin, stdout, shell, argv);
	if (ft_is_number(argv[1]) && !argv[2])
		exit_number(stdin, stdout, shell, argv);
	else
	{
		write(STDOUT_FILENO, "exit\n", 5);
		ft_putstr_fd("exit: too many arguments\n", 2);
		g_global()->exit_status = 1;
		return (g_global()->exit_status);
	}
	return (0);
}
