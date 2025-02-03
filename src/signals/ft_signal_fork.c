/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal_fork.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:24:47 by nrasamim          #+#    #+#             */
/*   Updated: 2025/02/03 11:48:15 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	handler_signal_pipe(pid_t pid)
{
	int	status;

	close(g_global()->pipfd[0]);
	waitpid(pid, &status, 0);
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

int	handler_signal_hdoc(int *pipe_fd, pid_t pid, t_cmd *cmd, int std_fds[2],
		int use_pipe)
{
	int	status;

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
			if (use_pipe)
				close_hdoc_fd_inherited_from_parent();
			retore_fds_standart(-1, -1, &std_fds[0], &std_fds[1]);
			return (write(STDOUT_FILENO, "\n", 1), -2);
		}
	}
	return (0);
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
