/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 10:57:19 by frahenin          #+#    #+#             */
/*   Updated: 2025/01/21 13:06:27 by frahenin         ###   ########.fr       */
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
		exit(0);
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
		shell->exit_status = ft_atoi(argv[1]);
		ft_free_all(shell);
		exit(shell->exit_status);
	}
	else
	{
		perror(argv[1]);
		return (1);
	}
}