/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:35:28 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/27 11:02:39 by nrasamim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_bool	check_new_line(char *str)
{
	int	i;

	i = 0;
	if (str[i] && str[i] == '-')
	{
		++i;
		while (str[i] && str[i] == 'n')
			i++;
		if (str[i] == '\0')
			return (TRUE);
	}
	return (FALSE);
}

static void	print_echo(int number_arg, char **args)
{
	int	i;
	t_bool	new_line;

	i = 1;
	if (!args[i])
		return ;
	new_line = TRUE;
	while(check_new_line(args[i]))
	{
		new_line = FALSE;
		i++;
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
