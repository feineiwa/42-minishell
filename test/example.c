/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   example.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 09:53:26 by frahenin          #+#    #+#             */
/*   Updated: 2024/12/31 11:54:03 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int	main(int ac, char **av)
{
	(void)ac;
	int status;

	if (fork() == 0)
		execvp(av[1], &av[1]);
	wait(&status);
	return (0);
}