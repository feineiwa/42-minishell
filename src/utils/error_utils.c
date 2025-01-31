/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:36:34 by frahenin          #+#    #+#             */
/*   Updated: 2025/01/31 16:28:28 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	print_err(char *s1, char *s2, char *s3, int fd)
{
	if (s1)
		ft_putstr_fd(s1, fd);
	if (s2)
		ft_putstr_fd(s2, fd);
	if (s3)
		ft_putstr_fd(s3, fd);
}

void	ft_free_pipe(int pipefd[2])
{
	close(pipefd[0]);
	close(pipefd[1]);
	ft_free(g_global()->hdoc_fd);
}

void	ft_free_arr(char **arr)
{
	int	i;

	i = 0;
	if (!arr | !*arr)
		return ;
	while (arr[i])
	{
		ft_free(arr[i]);
		i++;
	}
	ft_free(arr);
}
