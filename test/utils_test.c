/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_test.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 09:55:58 by frahenin          #+#    #+#             */
/*   Updated: 2024/12/30 10:19:59 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void	describe_group(char *groupe)
{
	if (!groupe)
		return ;
	P(GREEN "%s\n" RESET, groupe);
}

void	describe_test(char *msg)
{
	P(CYAN "%s\n" RESET, msg);
}

int	should_be_int(char *msg, int a, int b)
{
	P(YELLOW "%s " RESET, msg);
	if (a == b)
		P(GREEN "OK\n" RESET);
	else
	{
		P(RED "KO\n" RESET);
		return (0);
	}
	return (1);
}

int	should_be_str(char *msg, char *s1, char *s2)
{
	int	i;

	P(MAGENTA "%s " RESET, msg);
	i = 0;
	while (s1[i] == s2[i] && s1[i] && s2[i])
		i++;
	if (s1[i] == '\0' && s2[i] == '\0')
		return (P(GREEN "OK\n" RESET), 1);
	else
		return (P(RED "KO\n" RESET), 0);
	return (1);
}

int	should_be_ptr(char *msg, void *ptr1, void *ptr2)
{
	P(BLUE "%s ", msg);
	if (ptr1 == ptr2)
		return (P(GREEN "OK\n" RESET), 1);
	else
		return (P(RED "KO\n" RESET), 0);
	return (1);
}
