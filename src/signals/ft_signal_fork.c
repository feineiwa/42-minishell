/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal_fork.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:24:47 by nrasamim          #+#    #+#             */
/*   Updated: 2025/02/04 18:08:20 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	handler_signal_pipe(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	close(g_global()->pipfd[0]);
	close(g_global()->pipfd[1]);
	if (WIFSIGNALED(status))
	{
		g_global()->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
		{
			write(STDOUT_FILENO, "Quit (core dumped)\n", 20);
			return (g_global()->exit_status);
		}
	}
	else if (WIFEXITED(status))
		g_global()->exit_status = WEXITSTATUS(status);
	while (wait(&status) > 0)
		;
	if (WIFSIGNALED(status))
		if (WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
	return (g_global()->exit_status);
}

void	handler_signal_fork(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_global()->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		g_global()->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			write(STDOUT_FILENO, "Quit (core dumped)\n", 20);
		else if (WTERMSIG(status) == SIGINT)
			ft_putchar_fd('\n', STDOUT_FILENO);
	}
}

int	handler_signal_hdoc(int *pipe_fd, pid_t pid, t_cmd *cmd, int std_fds[2])
{
	int	status;

	(void)std_fds;
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		g_global()->exit_status = WEXITSTATUS(status);
		if (cmd->error_file)
			return (close(pipe_fd[0]), -1);
	}
	else if (WIFSIGNALED(status))
	{
		g_global()->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
		{
			close(pipe_fd[0]);
			if (g_global()->use_pipe)
				close_hdoc_fd_inherited_from_parent();
			return (write(STDOUT_FILENO, "\n", 1), -2);
		}
	}
	return (pipe_fd[0]);
}
