/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 11:30:42 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/15 14:22:34 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int  between_heredoc_and_cmd(t_hdoc *hdoc)
{
	char    *content;
    int     pipe_fd[2];
    pid_t   pid;

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
            close(pipe_fd[0]);
            write (pipe_fd[1], content, ft_strlen(content));
            write (pipe_fd[1], "\n", 1);
            ft_free(content);
        }
        exit(0);
    }
    else
    {
        int status;
        close(pipe_fd[1]);
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        {
            close(pipe_fd[0]);
            return (-1);
        }
    }
    return (pipe_fd[0]);
}

int   handle_heredoc(t_cmd *cmd)
{
    int     inputfd;
    t_hdoc  *hdoc;
    int     tmp_fd;
    int     saved_std;

    inputfd = -1;
    hdoc = cmd->hdoc;
    saved_std = dup(STDIN_FILENO);
    while (hdoc)
    {
        tmp_fd = between_heredoc_and_cmd(hdoc);
        if (tmp_fd < 0)
        {
            perror("minishell: read .heredoc.tmp");
            if (inputfd != -1)
                close(inputfd);
            return (-1);
        }
        if (inputfd != 1)
            close(inputfd);
        inputfd = tmp_fd;
        // ft_free(cmd->hdoc->del);
        // cmd->hdoc->del = NULL;
        hdoc = hdoc->next;
    }
    if (inputfd != -1 && dup2(inputfd, STDIN_FILENO) < 0)
    {
        perror("minishell: dup2 input");
        close(inputfd);
        return (-1);
    }
    return (inputfd);
}
