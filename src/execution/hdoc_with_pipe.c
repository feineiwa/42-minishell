/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hdoc_with_pipe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:52:32 by nrasamim          #+#    #+#             */
/*   Updated: 2025/02/05 12:10:29 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	handle_single_heredoc(t_cmd *tmp, t_shell *shell, int std_fds[2],
		int i)
{
	int	hdoc_ret;

	g_global()->shell = shell;
	hdoc_ret = -1;
	if (tmp->hdoc && tmp->hdoc->del)
	{
		hdoc_ret = handle_heredoc(tmp, shell, std_fds);
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

int	handle_heredoc_with_pipe(t_cmd *cmd, t_shell *shell, int std_fds[2])
{
	int		i;
	t_cmd	*tmp;

	i = 0;
	tmp = cmd;
	g_global()->shell = shell;
	while (tmp)
	{
		g_global()->is_runing = 2;
		setup_signal();
		if (handle_single_heredoc(tmp, shell, std_fds, i) == -2)
			return (1);
		i++;
		tmp = tmp->next;
	}
	return (0);
}
