/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 12:38:58 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/21 00:03:44 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_bool	config_with_pipe(t_shell *shell, t_cmd *cmd)
{
	int		pipefd[2];
	int		input_fd;
	int		pid;
	t_cmd	*tmp;
	int		*hdoc_fd;
	int		i;
	int	status;


	input_fd = -1;
	tmp = cmd;
	i = 0;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	hdoc_fd = malloc(sizeof(int) * i);
	if (!hdoc_fd)
		return (FALSE);
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
			return (FALSE);
		}
		i++;
		tmp = tmp->next;
	}
	tmp = cmd;
	i = 0;
	while (tmp)
	{
		ignore_sig();
		if (!tmp->argv[0])
			return (FALSE);
		if (tmp->next && pipe(pipefd) < 0)
		{
			perror("pipe");
			ft_free(hdoc_fd);
			return (FALSE);
		}
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			free(hdoc_fd);
			return (FALSE);
		}
		if (pid == 0) // Child process
		{
			signal(SIGINT, SIG_DFL);
			if (tmp->hdoc && tmp->hdoc->del)
				input_fd = hdoc_fd[i];
			if (input_fd != -1)
			{
				if (dup2(input_fd, STDIN_FILENO) < 0)
				{
					perror("dup2 pipe");
					return (FALSE);
				}
				close(input_fd);
			}
			if (tmp->next)
			{
				close(pipefd[0]);
				if (dup2(pipefd[1], STDOUT_FILENO) < 0)
				{
					perror("dup2");
					return (FALSE);
				}
				close(pipefd[1]);
			}
			if (!launch_cmd_with_pipe(shell, tmp))
                exit(1);
			exit(0);
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
			waitpid(pid, &status, 0);
			if (WIFSIGNALED(status))
        	{
            g_global()->exit_status = 128 + WTERMSIG(status);
            if (WTERMSIG(status) == SIGINT)
            {
                close(pipefd[0]);
				close(input_fd);
				close(pipefd[1]);
                // dup2(saved_stdin, STDIN_FILENO);
                // dup2(saved_stdout, STDOUT_FILENO);
                ft_putchar_fd('\n', 2);
                return (-2);
            }
        }
			
		}
		tmp = tmp->next;
		i++;
	}
	while (wait(NULL) > 0)
		;
	waitpid(pid, &status, 0);
    ft_free(hdoc_fd);
	return (TRUE);
}
