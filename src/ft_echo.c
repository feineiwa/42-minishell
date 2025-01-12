/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:35:28 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/12 20:18:01 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static t_bool	check_new_line(char *str)
{
	int	i;

	i = 0;
	if (str[i] && str[i] == '-')
	{
		++i;
		while (str[i] && str[i] == 'n')
			i++;
		if (i == (int)ft_strlen(str))
			return (TRUE);
	}
	return (FALSE);
}

static void	print_echo(int number_arg, char **args)
{
	int	i;
	t_bool	new_line;

	i = 1;
	new_line = FALSE;
	while (args[i] && check_new_line(args[i]))
	{
		i++;
		new_line = FALSE;
	}
	while (i < number_arg)
	{
		write(1, args[i], ft_strlen(args[i]));
		if (i != number_arg - 1)
			write(1, " ", 1);
		i++;
	}
	if (new_line)
		write(1, "\n", 1);
}

int	ft_echo(char **args)
{
	int		number_arg;

	number_arg = 0;
	while (args[number_arg])
		number_arg++;
	print_echo(number_arg, args);
	return (0);
}
