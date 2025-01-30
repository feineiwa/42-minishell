/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 11:30:42 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/30 17:48:13 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	read_heredoc(t_hdoc *hdoc, t_shell *shell, int pipe_fd[2])
{
	char	*content;
	char	*expand;

	while (42)
	{
		content = readline(HDOC);
		if (content == NULL)
		{
			ft_putendl_fd("warning: here-document delimited by end-of-file", 2);
			break ;
		}
		if (!ft_strcmp(content, hdoc->del))
		{
			ft_free(content);
			break ;
		}
		if (hdoc->expanded)
		{
			expand = ft_expand_for_hdoc(shell, content);
			ft_free(content);
			content = expand;
		}
		close(pipe_fd[0]);
		if (hdoc->next)
		{
			ft_free(content);
			continue ;
		}
		else
		{
			write(pipe_fd[1], content, ft_strlen(content));
			write(pipe_fd[1], "\n", 1);
			ft_free(content);
		}
	}
}

static int	between_heredoc_and_cmd(t_hdoc *hdoc, t_cmd *cmd, t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipe_fd) < 0)
	{
		perror("pipe");
		return (-1);
	}
	setup_signal();
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	else if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		close(pipe_fd[0]);
		while (hdoc)
		{
			read_heredoc(hdoc, shell, pipe_fd);
			hdoc = hdoc->next;
		}
		exit(0);
	}
	else
	{
		status = handler_signal_hdoc(pipe_fd, pid, cmd);
		if (status < 0)
			return (status);
	}
	return (pipe_fd[0]);
}

int	handle_heredoc(t_cmd *cmd, t_shell *shell)
{
	int	inputfd;

	inputfd = -1;
	inputfd = between_heredoc_and_cmd(cmd->hdoc, cmd, shell);
	return (inputfd);
}

int	*handle_heredoc_with_pipe(t_cmd *cmd, t_shell *shell)
{
	int		i;
	int		*hdoc_fd;
	t_cmd	*tmp;

	hdoc_fd = malloc(sizeof(int) * ft_cmdsize(cmd));
	if (!hdoc_fd)
		return (NULL);
	tmp = cmd;
	i = 0;
	while (tmp)
	{
		g_global()->is_runing = 2;
		setup_signal();
		if (tmp->hdoc && tmp->hdoc->del)
			hdoc_fd[i] = handle_heredoc(tmp, shell);
		else
			hdoc_fd[i] = -1;
		if (hdoc_fd[i] == -2)
		{
			ft_free(hdoc_fd);
			return (NULL);
		}
		i++;
		tmp = tmp->next;
	}
	return (hdoc_fd);
}
