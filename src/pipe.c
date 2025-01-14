/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 12:38:58 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/14 09:08:04 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_bool    config_with_pipe(t_shell *shell, t_cmd *cmd)
{
    int pipefd[2];
    int input_fd;
    pid_t   pid;

    input_fd = -1;
    while (cmd)
    {
        if (!cmd->argv[0])
            return (FALSE);
        if (pipe(pipefd) < 0)
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
            if (input_fd != -1)
            {
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }
            if (cmd->next)
            {
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
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
