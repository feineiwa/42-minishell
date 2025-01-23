/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 11:30:42 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/23 13:22:14 by nrasamim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	between_heredoc_and_cmd(t_hdoc *hdoc, t_cmd *cmd, t_shell *shell)
{
	char	*content;
	int		pipe_fd[2];
	pid_t	pid;
	char	*expand;
	int		in_fd;
	int		status;


	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	else if (pid == 0)
	{
		g_global()->is_runing = 0;
		setup_signal();
		while (hdoc)
		{
			while (42)
			{
				content = readline(HDOC);
				if (content == NULL)
				{
					ft_putendl_fd("warning: here-document delimited by end-of-file",
						2);
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
			hdoc = hdoc->next;
		}
		exit(0);
	}
	else
	{
		g_global()->is_runing = 2;
		setup_signal();
		close(pipe_fd[1]);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			g_global()->exit_status = WEXITSTATUS(status);
			if (cmd->input_file)
			{
				close(pipe_fd[0]);
				if ((in_fd = open(cmd->input_file, O_RDONLY) < 0))
					return (-1);
				close(in_fd);
			}
		}
		else if (WIFSIGNALED(status))
		{
			g_global()->exit_status = 128 + WTERMSIG(status);
			if (WTERMSIG(status) == SIGINT)
			{
				close(pipe_fd[0]);
				close(pipe_fd[1]);
				write (STDOUT_FILENO, "\n", 1);
				return (-2);
			}
		}
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
