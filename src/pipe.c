/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 12:38:58 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/14 16:53:29 by nrasamim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_bool config_with_pipe(t_shell *shell, t_cmd *cmd)
{
    int pipefd[2];
    int input_fd;
    pid_t pid;

    input_fd = -1;
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
        if (pid == 0) // Child Process
        {
            if (cmd->hdoc && cmd->hdoc->del)
            {
                input_fd = handle_heredoc(cmd);
                if (dup2(input_fd, STDIN_FILENO) < 0)
                {
                    perror("minishell: dup2 heredoc");
                    return (FALSE);
                }
                close(input_fd);
                cmd->hdoc->del = NULL;
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
            launch_cmd(shell, cmd);
            exit(1);
        }
        else // Parent Process
        {
            if (input_fd != -1)
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
