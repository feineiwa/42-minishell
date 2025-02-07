/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 12:38:58 by nrasamim          #+#    #+#             */
/*   Updated: 2025/02/07 19:04:27 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_bool	update_std_fds(int *in, t_cmd *cmd, int fd)
{
	if (*in != -1)
	{
		if (dup2(*in, STDIN_FILENO) < 0)
		{
			perror("dup2");
			return (FALSE);
		}
		close(*in);
		*in = -1;
	}
	if (fd != -1)
	{
		if (dup2(fd, STDIN_FILENO) < 0)
		{
			perror("dup2 pipe");
			return (FALSE);
		}
		close(fd);
	}
	if (cmd->next)
	{
		if (!cmd->next->hdoc && !cmd->next->input_file)
		{
			if (g_global()->pipfd[0] != -1)
				close(g_global()->pipfd[0]);
		}
		if (dup2(g_global()->pipfd[1], STDOUT_FILENO) < 0)
		{
			perror("dup2");
			return (FALSE);
		}
		close(g_global()->pipfd[1]);
	}
	return (TRUE);
}

static void	child_process(int *in, t_cmd *cmd, int hdoc_fd, int sa_std[2])
{
	g_global()->use_pipe = TRUE;
	g_global()->is_runing = 1;
	setup_signal();
	if (!update_std_fds(in, cmd, hdoc_fd))
	{
		if (g_global()->pipfd[0] != -1)
			close(g_global()->pipfd[0]);
		if (g_global()->pipfd[1] != -1)
			close(g_global()->pipfd[1]);
		close_hdoc_fd_inherited_from_parent();
		close_saved_std(sa_std);
		ft_free_all(g_global()->shell);
		exit(1);
	}
	g_global()->exit_status = launch_cmd(g_global()->shell, cmd, sa_std);
	if (g_global()->pipfd[1] != -1)
		close(g_global()->pipfd[1]);
	close_hdoc_fd_inherited_from_parent();
	close_saved_std(sa_std);
	ft_free_all(g_global()->shell);
	exit(g_global()->exit_status);
}

static int	parent_process(int *in, int hdoc_fd, t_cmd *cmd)
{
	close(g_global()->pipfd[1]);
	if (*in != -1)
	{
		close(*in);
		*in = -1;
	}
	if (cmd->next)
	{
		*in = dup(g_global()->pipfd[0]);
		if (*in == -1)
			return (perror("dup pipe read end"), 1);
		close(g_global()->pipfd[0]);
		g_global()->pipfd[0] = -1;
	}
	if (g_global()->pipfd[0] != -1)
	{
		close(g_global()->pipfd[0]);
		g_global()->pipfd[0] = -1;
	}
	if (hdoc_fd != -1)
	{
		close(hdoc_fd);
		hdoc_fd = -1;
	}
	return (0);
}

static int	create_pipe_and_fork(pid_t *pid)
{
	g_global()->pipfd[0] = -1;
	g_global()->pipfd[1] = -1;
	if (pipe((g_global()->pipfd)) < 0)
	{
		perror("pipe");
		return (1);
	}
	*pid = fork();
	if (*pid < 0)
	{
		close(g_global()->pipfd[0]);
		close(g_global()->pipfd[1]);
		perror("fork");
		return (1);
	}
	return (0);
}

int	config_with_pipe(t_shell *shell, t_cmd *cmd, int sa_std[2])
{
	int		i;
	pid_t	pid;
	int		in;

	in = -1;
	g_global()->shell = shell;
	setup_signal();
	if (handle_heredoc_with_pipe(cmd, shell, sa_std))
		return (g_global()->exit_status);
	i = 0;
	while (cmd)
	{
		if ((cmd->next && !g_global()->pipfd) || create_pipe_and_fork(&pid))
			return (restore_standard(sa_std), 1);
		if (pid == 0)
			child_process(&in, cmd, g_global()->hdoc_fd[i], sa_std);
		else if (parent_process(&in, g_global()->hdoc_fd[i], cmd))
			return (1);
		cmd = cmd->next;
		i++;
	}
	return (handler_signal_pipe(pid));
}
