/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 12:38:58 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/30 17:46:26 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static t_bool	update_std_fds(t_cmd *cmd, int *pipefd, int fd)
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

static void	child_process(t_shell *shell, t_cmd *cmd, int *pipefd,
		int *input_fd, int hdoc_fd)
{
	g_global()->is_runing = 1;
	setup_signal();
	if (cmd->hdoc && cmd->hdoc->del)
		*input_fd = hdoc_fd;
	if (!update_std_fds(cmd, pipefd, *input_fd))
		exit(1);
	g_global()->exit_status = launch_cmd(shell, cmd, 1);
	if (!cmd->argv[0])
	{
		close(pipefd[1]);
		close(pipefd[0]);
	}
	exit(g_global()->exit_status);
}

static void	parent_process(int *pipefd, int *input_fd, int hdoc_fd, t_cmd *cmd)
{
	if (*input_fd != -1 && *input_fd != hdoc_fd)
		close(*input_fd);
	if (cmd->next)
	{
		close(pipefd[1]);
		*input_fd = pipefd[0];
	}
	else
		*input_fd = -1;
	if (hdoc_fd != -1)
		close(hdoc_fd);
}

static int	*create_pipe_and_fork(pid_t *pid, int *hdoc_fd)
{
	int	*pipefd;

	pipefd = (int *)malloc(sizeof(int) * 2);
	if (!pipefd)
	{
		perror("malloc");
		ft_free(hdoc_fd);
		return (NULL);
	}
	if (pipe(pipefd) < 0)
	{
		perror("pipe");
		ft_free(pipefd);
		ft_free(hdoc_fd);
		return (NULL);
	}
	*pid = fork();
	if (*pid < 0)
	{
		perror("fork");
		ft_free(pipefd);
		ft_free(hdoc_fd);
		return (NULL);
	}
	return (pipefd);
}

int	config_with_pipe(t_shell *shell, t_cmd *cmd)
{
	int		i;
	int		input_fd;
	int		*pipefd;
	int		*hdoc_fd;
	pid_t	pid;

	hdoc_fd = handle_heredoc_with_pipe(cmd, shell);
	if (hdoc_fd == NULL)
		return (g_global()->exit_status);
	setup_signal();
	i = 0;
	input_fd = -1;
	while (cmd)
	{
		pipefd = create_pipe_and_fork(&pid, hdoc_fd);
		if (cmd->next && !pipefd)
			return (1);
		if (pid == 0)
			child_process(shell, cmd, pipefd, &input_fd, hdoc_fd[i]);
		else
			parent_process(pipefd, &input_fd, hdoc_fd[i], cmd);
		cmd = cmd->next;
		i++;
	}
	return (ft_free_pipe(pipefd, hdoc_fd), handler_signal_pipe(pid));
}
