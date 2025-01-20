/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 11:30:42 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/20 19:13:10 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int  between_heredoc_and_cmd(t_hdoc *hdoc, t_cmd *cmd, t_shell *shell)
{
	char    *content;
    int     pipe_fd[2];
    pid_t   pid;
    char    *expand;
    int     in_fd;
    int     saved_stdout;

    saved_stdout = dup(STDOUT_FILENO);
    ignore_sig();
    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        return (-1);
    }
    in_fd = -1;
    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return (-1);
    }
    else if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        while (hdoc)
        {
            while (42)
            {
                content = readline(HDOC);
                if (content == NULL)
                {
                    perror("Warning : There're not delimiter in the heredoc");
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
                    write (pipe_fd[1], content, ft_strlen(content));
                    write (pipe_fd[1], "\n", 1);
                    ft_free(content);
                }
            }
            hdoc = hdoc->next;
        }
        exit(0);
    }
    else
    {
        int     status;
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
                dup2(saved_stdout, STDOUT_FILENO);
                ft_putchar_fd('\n', 1);
                return (-1);
            }
        }
    }
    return (pipe_fd[0]);
}

int   handle_heredoc(t_cmd *cmd, t_shell *shell)
{
    int     inputfd;

    inputfd = -1;
    inputfd = between_heredoc_and_cmd(cmd->hdoc, cmd, shell);
    return (inputfd);
}
