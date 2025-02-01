/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 12:38:58 by nrasamim          #+#    #+#             */
/*   Updated: 2025/02/01 15:04:35 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static t_bool	update_std_fds(t_cmd *cmd, int fd)
{
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
		if (dup2(g_global()->pipfd[1], STDOUT_FILENO) < 0)
		{
			perror("dup2");
			return (FALSE);
		}
		close(g_global()->pipfd[1]);
	}
	return (TRUE);
}

static void	child_process(t_shell *shell, t_cmd *cmd, int *input_fd,
		int hdoc_fd)
{
	g_global()->is_runing = 1;
	setup_signal();
	close(g_global()->pipfd[0]);
	if (cmd->hdoc && cmd->hdoc->del)
		*input_fd = hdoc_fd;
	if (!update_std_fds(cmd, *input_fd))
	{
		ft_free_all(shell);
		exit(1);
	}
	g_global()->exit_status = launch_cmd(shell, cmd, 1);
	if (!cmd->argv[0])
		close(g_global()->pipfd[1]);
	ft_free_all(shell);
	exit(g_global()->exit_status);
}

static void	parent_process(int *input_fd, int hdoc_fd, t_cmd *cmd)
{
	close(g_global()->pipfd[1]);
	if (*input_fd != -1 && *input_fd != hdoc_fd)
		close(*input_fd);
	if (cmd->next)
		*input_fd = g_global()->pipfd[0];
	else
		*input_fd = -1;
	if (hdoc_fd != -1)
		close(hdoc_fd);
}

static int	create_pipe_and_fork(pid_t *pid)
{
	if (pipe((g_global()->pipfd)) < 0)
	{
		perror("pipe");
		return (1);
	}
	*pid = fork();
	if (*pid < 0)
	{
		perror("fork");
		return (1);
	}
	return (0);
}

int	config_with_pipe(t_shell *shell, t_cmd *cmd)
{
	int		i;
	int		input_fd;
	pid_t	pid;
	int		std_fds[2];

	std_fds[0] = -1;
	std_fds[1] = -1;
	if (handle_heredoc_with_pipe(cmd, shell, std_fds))
		return (g_global()->exit_status);
	setup_signal();
	i = -1;
	input_fd = -1;
	while (cmd)
	{
		if (create_pipe_and_fork(&pid))
			return (1);
		if (cmd->next && !g_global()->pipfd)
			return (1);
		if (pid == 0)
			child_process(shell, cmd, &input_fd, g_global()->hdoc_fd[++i]);
		else
			parent_process(&input_fd, g_global()->hdoc_fd[++i], cmd);
		cmd = cmd->next;
	}
	return (ft_free_pipe(g_global()->pipfd), handler_signal_pipe(pid));
}
