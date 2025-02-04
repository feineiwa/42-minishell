/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 12:38:58 by nrasamim          #+#    #+#             */
/*   Updated: 2025/02/04 21:17:58 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_bool	update_std_fds(t_cmd *cmd, int fd)
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

void	child_process(t_shell *shell, t_cmd *cmd, int *input_fd, int hdoc_fd,
		int sa_std[2])
{
	g_global()->use_pipe = TRUE;
	g_global()->is_runing = 1;
	setup_signal();
	close(g_global()->pipfd[0]);
	if (cmd->hdoc && cmd->hdoc->del)
	{
		if (*input_fd != -1)
			close(*input_fd);
		*input_fd = hdoc_fd;
	}
	if (!update_std_fds(cmd, *input_fd))
	{
		close(g_global()->pipfd[1]);
		close_hdoc_fd_inherited_from_parent();
		close_saved_std(sa_std);
		ft_free_all(shell);
		exit(1);
	}
	g_global()->exit_status = launch_cmd(shell, cmd, sa_std);
	close_hdoc_fd_inherited_from_parent();
	close(g_global()->pipfd[1]);
	close_saved_std(sa_std);
	ft_free_all(shell);
	exit(g_global()->exit_status);
}

void	parent_process(int *input_fd, int hdoc_fd, t_cmd *cmd)
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

int	create_pipe_and_fork(pid_t *pid)
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

int	config_with_pipe(t_shell *shell, t_cmd *cmd, int sa_std[2])
{
	int		i;
	int		input_fd;
	pid_t	pid;

	g_global()->shell = shell;
	setup_signal();
	if (handle_heredoc_with_pipe(cmd, shell, sa_std))
		return (restore_standard(sa_std), g_global()->exit_status);
	i = 0;
	input_fd = -1;
	while (cmd)
	{
		if ((cmd->next && !g_global()->pipfd) || create_pipe_and_fork(&pid))
			return (restore_standard(sa_std), 1);
		if (pid == 0)
			child_process(shell, cmd, &input_fd, g_global()->hdoc_fd[i],
				sa_std);
		else
			parent_process(&input_fd, g_global()->hdoc_fd[i], cmd);
		cmd = cmd->next;
		i++;
	}
	handler_signal_pipe(pid);
	restore_standard(sa_std);
	return (0);
}
