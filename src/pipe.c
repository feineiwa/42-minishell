/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 12:38:58 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/17 12:55:21 by nrasamim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_bool    config_with_pipe(t_shell *shell, t_cmd *cmd)
{
    int pipefd[2];
    int input_fd;
    pid_t   pid;
    int     herdoc_fd;
    t_cmd   *tmp;

    input_fd = -1;
    close(input_fd);
    tmp = cmd;
    while (cmd)
    {
        if (!cmd->argv[0])
            return (FALSE);
        if (cmd->next && pipe(pipefd) < 0)
        {
            perror("minishell: pipe");
            return (FALSE);
        }
        pid = fork();
        if (pid < 0)
        {
            perror("minishell: fork");
            return (FALSE);
        }
        else if (pid == 0) // Child Process
        {
            if (cmd->hdoc && cmd->hdoc->del)
            {
                input_fd = handle_heredoc(cmd, shell);
                if (dup2(input_fd, STDIN_FILENO) < 0)
                {
                    perror("minishell: dup2 heredoc");
                    return (FALSE);
                }
                close(input_fd);
            }
            else if (input_fd != -1)
            {
                if (dup2(input_fd, STDIN_FILENO) < 0)
                {
                    perror("minishell: dup2 heredoc");
                    return (FALSE);
                }
                close(input_fd);
            }
            if (cmd->next)
            {
                close(pipefd[0]);
                if (dup2(pipefd[1], STDOUT_FILENO) < 0)
                {
                    perror("minishell: dup2 heredoc");
                    return (FALSE);
                }
                close(pipefd[1]);
            }
            launch_cmd_with_pipe(shell, cmd);
            exit(1);
        }
        else // Parent Process
        {
            if (input_fd >= 0)
                close(input_fd);
            if (cmd->next)
                close(pipefd[1]);
            input_fd = pipefd[0];
            waitpid(pid, &shell->exit_status, 0);
        }
        cmd = cmd->next;
    }
    return (TRUE);
}
