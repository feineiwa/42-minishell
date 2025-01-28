/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 12:38:58 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/28 17:17:20 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static t_bool	child_process(t_cmd *cmd, int *pipefd, int fd)
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
		close(pipefd[0]);
		if (dup2(pipefd[1], STDOUT_FILENO) < 0)
		{
			perror("dup2");
			return (FALSE);
		}
		close(pipefd[1]);
	}
	return (TRUE);
}

int	config_with_pipe(t_shell *shell, t_cmd *cmd)
{
	int		pipefd[2];
	int		input_fd;
	int		*hdoc_fd;
	int		i;
	int		status;
	t_cmd	*tmp;
	pid_t	pid;
	
	hdoc_fd = handle_heredoc_with_pipe(cmd, shell);
	if (hdoc_fd == NULL)
		return (g_global()->exit_status);
	input_fd = -1;
	i = 0;
	setup_signal();
	tmp = cmd;
	while (tmp)
	{
		if (tmp->next && pipe(pipefd) < 0)
		{
			perror("pipe");
			ft_free(hdoc_fd);
			return (1);
		}
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			free(hdoc_fd);
			return (1);
		}
		if (pid == 0)
		{
			g_global()->is_runing = 1;
			setup_signal();
			if (tmp->hdoc && tmp->hdoc->del)
				input_fd = hdoc_fd[i];
			if (!child_process(tmp, pipefd, input_fd))
				exit(1);
			g_global()->exit_status = launch_cmd_with_pipe(shell, tmp);
			if (!tmp->argv[0])
			{
				close(pipefd[1]);
				close(pipefd[0]);
			}
			exit(g_global()->exit_status);
		}
		else
		{
			if (input_fd != -1 && input_fd != hdoc_fd[i])
				close(input_fd);
			if (tmp->next)
				close(pipefd[1]);
			if (tmp->next)
				input_fd = pipefd[0];
			else
				input_fd = -1;
			if (hdoc_fd[i] != -1)
				close(hdoc_fd[i]);
		}
		tmp = tmp->next;
		i++;
	}
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
	{
		if (WIFSIGNALED(status))
			if (WTERMSIG(status) == SIGINT)
				write(STDOUT_FILENO, "\n", 1);
	}
	ft_free(hdoc_fd);
	return (g_global()->exit_status);
}
