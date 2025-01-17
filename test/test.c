/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 09:52:43 by frahenin          #+#    #+#             */
/*   Updated: 2025/01/14 12:15:42 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int	main(void)
{
	char *str[] = {"cadfasdf",
		"asdshhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhssssssssssssssssssssfasdf",
		"asdfsadfasd", NULL};
	double size;

	size = sizeof(str) / sizeof(char *);
	int i;
	i = 0;
	while (str[i])
		i++;
	printf("size %f et %d\n", size, i);
	size = sizeof() / sizeof(char);
	printf("size %f", size);
	return (0);
}