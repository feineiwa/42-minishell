/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 11:30:42 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/16 11:05:16 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int  between_heredoc_and_cmd(t_hdoc *hdoc, t_shell *shell)
{
	char    *content;
    int     pipe_fd[2];
    pid_t   pid;
    char    *expand;

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
        while (shell->cmd)
        {
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
                    write (pipe_fd[1], content, ft_strlen(content));
                    write (pipe_fd[1], "\n", 1);
                    ft_free(content);
                }
                if (hdoc->next->del)
                {
                    hdoc = hdoc->next;
                    continue ;
                }
            }
            if (shell->cmd->next)
            {
                shell->cmd = shell->cmd->next;
                continue ;
            }
            exit(0);
        }
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

int   handle_heredoc(t_cmd *cmd, t_shell *shell)
{
    int     inputfd;

    inputfd = -1;
    inputfd = between_heredoc_and_cmd(cmd->hdoc, shell);
    if (inputfd != -1 && dup2(inputfd, STDIN_FILENO) < 0)
    {
        perror("minishell: dup2 input");
        close(inputfd);
        return (-1);
    }
    return (inputfd);
}
