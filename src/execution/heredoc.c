/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 11:30:42 by nrasamim          #+#    #+#             */
/*   Updated: 2025/02/05 15:13:00 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	read_heredoc(t_hdoc *hdoc, pid_t pid, int pipe_fd[2],
		int std_fds[2])
{
	char	*content;

	while (42)
	{
		content = readline(HDOC);
		handle_ctrl_c(content, pipe_fd, std_fds, pid);
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
		content = expand_content_if_needed(content, hdoc);
		if (hdoc->next)
		{
			ft_free(content);
			continue ;
		}
		else
			write_content_to_pipe(content, pipe_fd);
	}
}

static void	child_process_for_hdoc(t_hdoc *hdoc, int pipe_fd[2], pid_t pid,
		int std_fds[2])
{
	g_global()->is_runing = 3;
	setup_signal_for_hdoc();
	close(pipe_fd[0]);
	if (g_global()->use_pipe)
		close_hdoc_fd_inherited_from_parent();
	while (hdoc)
	{
		read_heredoc(hdoc, pid, pipe_fd, std_fds);
		hdoc = hdoc->next;
	}
	close(pipe_fd[1]);
	close(std_fds[0]);
	close(std_fds[1]);
	ft_free_all(g_global()->shell);
	exit(0);
}

static int	between_heredoc_and_cmd(t_hdoc *hdoc, t_cmd *cmd, int std_fds[2],
		t_shell *shell)
{
	int		pipe_fd[2];
	int		status;
	pid_t	pid;

	setup_signal();
	g_global()->shell = shell;
	if (pipe(pipe_fd) < 0)
		return (perror("pipe"), -1);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), -1);
	else if (pid == 0)
		child_process_for_hdoc(hdoc, pipe_fd, pid, std_fds);
	else
	{
		status = handler_signal_hdoc(pipe_fd, pid, cmd, std_fds);
		if (status < 0)
			return (status);
	}
	return (pipe_fd[0]);
}

int	handle_heredoc(t_cmd *cmd, t_shell *shell, int std_fds[2])
{
	int	inputfd;

	inputfd = -1;
	g_global()->shell = shell;
	inputfd = between_heredoc_and_cmd(cmd->hdoc, cmd, std_fds, shell);
	return (inputfd);
}
