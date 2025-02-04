/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:51:47 by nrasamim          #+#    #+#             */
/*   Updated: 2025/02/04 17:51:18 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_free(void *ptr)
{
	if (!ptr)
		return ;
	free(ptr);
	ptr = NULL;
}

void	ft_free_all(t_shell *shell)
{
	ft_free_cmd(&shell->cmd);
	ft_free_env(&shell->envp);
	if (g_global()->hdoc_fd != NULL)
	{
		ft_free(g_global()->hdoc_fd);
		g_global()->hdoc_fd = NULL;
	}
	rl_clear_history();
	if (g_global()->pipfd[1] != -1)
		close(g_global()->pipfd[1]);
	if (g_global()->pipfd[0] != -1)
		close(g_global()->pipfd[0]);
}
