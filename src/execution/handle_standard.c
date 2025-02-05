/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_standard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 12:31:53 by frahenin          #+#    #+#             */
/*   Updated: 2025/02/05 12:32:35 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	save_fds_standart(int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
}

void	restore_standard(int sa_std[2])
{
	if (sa_std[0] != -1)
	{
		if (dup2(sa_std[0], STDIN_FILENO) < 0)
			perror("dup2");
		close(sa_std[0]);
	}
	if (sa_std[1] != -1)
	{
		if (dup2(sa_std[1], STDOUT_FILENO) < 0)
			perror("dup2");
		close(sa_std[1]);
	}
}

void	close_saved_std(int sa_std[2])
{
	if (sa_std[0] != -1)
	{
		close(sa_std[0]);
		sa_std[0] = -1;
	}
	if (sa_std[1] != -1)
	{
		close(sa_std[1]);
		sa_std[1] = -1;
	}
}
