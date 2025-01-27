/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 10:57:19 by frahenin          #+#    #+#             */
/*   Updated: 2025/01/27 18:53:13 by frahenin         ###   ########.fr       */
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
		exit(g_global()->exit_status);
	}
	else if (argv[2])
	{
		write(STDERR_FILENO, "exit\n", 5);
		perror(argv[2]);
		return (1);
	}
	if (ft_is_number(argv[1]))
	{
		close(stdin);
		close(stdout);
		g_global()->exit_status = ft_atoi(argv[1]);
		ft_free_all(shell);
		exit(g_global()->exit_status);
	}
	else
	{
		perror(argv[1]);
		return (1);
	}
	return (0);
}
