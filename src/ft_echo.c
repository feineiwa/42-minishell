/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:35:28 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/07 12:22:07 by nrasamim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static bool	check_new_line(char *str)
{
	int	i;

	i = 0;
	if (str[i] && str[i] == '-')
	{
		++i;
		while (str[i] && str[i] == 'n')
			i++;
		if (i == (int)ft_strlen(str))
			return (true);
	}
	return (false);
}

static void	print_echo(int count, char **args)
{
	int	i;
	bool	new_line;

	i = 1;
	new_line = true;
	while (args[i] && check_new_line(args[i]))
	{
		i++;
		new_line = false;
	}
	while (i < count)
	{
		printf("%s", args[i]);
		if (i != count - 1)
			printf(" ");
		i++;
	}
	if (new_line)
		printf("\n");
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
