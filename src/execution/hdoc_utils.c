/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hdoc_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 10:34:05 by nrasamim          #+#    #+#             */
/*   Updated: 2025/02/03 16:19:12 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	close_unused_hdoc_fd(int hdoc_fd)
{
	t_cmd	*tmp;
	int		j;

	tmp = g_global()->shell->cmd;
	j = 0;
	while (tmp)
	{
		if (g_global()->hdoc_fd[j] == hdoc_fd)
		{
			j++;
			tmp = tmp->next;
			continue ;
		}
		if (g_global()->hdoc_fd[j] != -1)
		{
			close(g_global()->hdoc_fd[j]);
			g_global()->hdoc_fd[j] = -1;
		}
		j++;
		tmp = tmp->next;
	}
}

void	close_hdoc_fd_inherited_from_parent(void)
{
	int		i;
	t_cmd	*tmp;

	i = 0;
	tmp = g_global()->shell->cmd;
	while (tmp)
	{
		if (g_global()->hdoc_fd[i] != -1)
			close(g_global()->hdoc_fd[i]);
		i++;
		tmp = tmp->next;
	}
}

void	handle_ctrl_c(char *content, int pipe_fd[2], int std_fds[2], pid_t pid)
{
	if (g_global()->is_runing == SIGINT)
	{
		ft_free(content);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		retore_fds_standart(-1, -1, &std_fds[0], &std_fds[1]);
		ft_free_all(g_global()->shell);
		signal(SIGINT, SIG_DFL);
		kill(pid, SIGINT);
	}
}

char	*expand_content_if_needed(char *content, t_hdoc *hdoc)
{
	char	*expand;

	expand = content;
	if (hdoc->expanded)
	{
		expand = ft_expand_for_hdoc(g_global()->shell, content);
		ft_free(content);
	}
	return (expand);
}

void	write_content_to_pipe(char *content, int pipe_fd[2])
{
	write(pipe_fd[1], content, ft_strlen(content));
	write(pipe_fd[1], "\n", 1);
	ft_free(content);
}
