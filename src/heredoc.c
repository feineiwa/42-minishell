/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 11:30:42 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/15 13:16:48 by nrasamim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int  between_heredoc_and_cmd(t_hdoc *hdoc)
{
	char    *content;
    int pipe_fd[2];
    pid_t   pid;

    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        return (-1);
    }
    pid = fork();
    if (pid == 0)
    {
        while (42)
        {
            content = readline("> ");
            if (content == NULL)
            {
                perror("Warning : There're not delimiter in the heredoc");
                break ;
            }
            if (!ft_strcmp(content, hdoc->del))
                break ;
            close(pipe_fd[0]);
            write(pipe_fd[1], content, ft_strlen(content));
            write(pipe_fd[1], "\n", 1);
            ft_free(content);
        }
        ft_free(content);
        exit(0);
    }
    else
    {
        waitpid(pid, NULL, 0);
	    close(pipe_fd[1]);
    }
    return (pipe_fd[0]);
}

int    handle_heredoc(t_cmd *cmd)
{
    int inputfd;

    inputfd = between_heredoc_and_cmd(cmd->hdoc);
    if (dup2(inputfd, STDIN_FILENO) < 0)
    {
        perror("minishell: dup2 input");
        close(inputfd);
        return (-1);
    }
    return (inputfd);
}
