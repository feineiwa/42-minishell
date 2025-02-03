/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hdoc_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 16:52:45 by nrasamim          #+#    #+#             */
/*   Updated: 2025/02/03 17:13:29 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	handle_single_heredoc(t_cmd *tmp, t_shell *shell, int std_fds[2], int i)
{
	int	hdoc_ret;

	hdoc_ret = -1;
	if (tmp->hdoc && tmp->hdoc->del)
	{
		hdoc_ret = handle_heredoc(tmp, shell, std_fds, 1);
		g_global()->hdoc_fd[i] = hdoc_ret;
	}
	else
		g_global()->hdoc_fd[i] = -1;
	return (hdoc_ret);
}

int	init_global_hdoc_fd(t_cmd *cmd)
{
	int	i;

	i = 0;
	g_global()->hdoc_fd = malloc(sizeof(int) * ft_cmdsize(cmd));
	if (!g_global()->hdoc_fd)
		return (0);
	while (cmd)
	{
		g_global()->hdoc_fd[i] = -1;
		i++;
		cmd = cmd->next;
	}
	return (1);
}
