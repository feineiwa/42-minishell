/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 11:30:42 by nrasamim          #+#    #+#             */
/*   Updated: 2025/02/02 16:58:03 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	read_heredoc(t_hdoc *hdoc, pid_t pid, int pipe_fd[2],
		int std_fds[2])
{
	char	*content;
	char	*expand;

	(void)pid;
	while (42)
	{
		content = NULL;
		content = readline(HDOC);
		if (content == NULL)
		{
			ft_putendl_fd("warning: here-document delimited by end-of-file", 2);
			break ;
		}
		if (g_global()->is_runing == SIGINT)
		{
			ft_free(content);
			content = NULL;
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			retore_fds_standart(-1, -1, &std_fds[0], &std_fds[1]);
			ft_free_all(g_global()->shell);
			exit(130);
		}
		if (!ft_strcmp(content, hdoc->del))
		{
			ft_free(content);
			break ;
		}
		if (hdoc->expanded)
		{
			expand = ft_expand_for_hdoc(g_global()->shell, content);
			ft_free(content);
			content = expand;
		}
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

void	handle_sigint_for_hdoc(int sig)
{
	if (sig == SIGINT)
	{
		g_global()->is_runing = SIGINT;
		close(0);
		close(1);
		close(2);
	}
	else
		g_global()->is_runing = 3;
}

void	setup_signal_for_hdoc(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_flags = 0;
	sa_quit.sa_flags = 0;
	sigemptyset(&sa_int.sa_mask);
	sigemptyset(&sa_quit.sa_mask);
	sa_int.sa_handler = &handle_sigint_for_hdoc;
	sa_quit.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa_int, NULL);
}

static int	between_heredoc_and_cmd(t_hdoc *hdoc, t_cmd *cmd, int std_fds[2],
		int use_pipe)
{
	int		pipe_fd[2];
	int		status;
	pid_t	pid;

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
		g_global()->is_runing = 3;
		setup_signal_for_hdoc();
		close(pipe_fd[0]);
		if (use_pipe)
			close_hdoc_fd_inherited_from_parent();
		while (hdoc)
		{
			read_heredoc(hdoc, pid, pipe_fd, std_fds);
			hdoc = hdoc->next;
		}
		close(pipe_fd[1]);
		retore_fds_standart(-1, -1, &std_fds[0], &std_fds[1]);
		ft_free_all(g_global()->shell);
		exit(0);
	}
	else
	{
		status = handler_signal_hdoc(pipe_fd, pid, cmd, std_fds, use_pipe);
		if (status < 0)
			return (status);
	}
	return (pipe_fd[0]);
}

int	handle_heredoc(t_cmd *cmd, t_shell *shell, int std_fds[2], int use_pipe)
{
	int	inputfd;

	inputfd = -1;
	g_global()->shell = shell;
	inputfd = between_heredoc_and_cmd(cmd->hdoc, cmd, std_fds, use_pipe);
	return (inputfd);
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
	int		hdoc_ret;

	if (!init_global_hdoc_fd(cmd))
		return (1);
	i = 0;
	tmp = cmd;
	while (tmp)
	{
		g_global()->is_runing = 2;
		setup_signal();
		if (tmp->hdoc && tmp->hdoc->del)
		{
			hdoc_ret = handle_heredoc(tmp, shell, std_fds, 1);
			g_global()->hdoc_fd[i] = hdoc_ret;
		}
		else
			g_global()->hdoc_fd[i] = -1;
		if (g_global()->hdoc_fd[i] == -2)
		{
			ft_free_cmd(&shell->cmd);
			ft_free(g_global()->hdoc_fd);
			return (1);
		}
		i++;
		tmp = tmp->next;
	}
	return (0);
}
