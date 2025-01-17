/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 12:38:58 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/17 16:56:07 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_bool config_with_pipe(t_shell *shell, t_cmd *cmd)
{
    int     pipefd[2];
    int     input_fd;
	int		pid;
    t_cmd   *tmp;

	input_fd = -1;
    tmp = cmd;
    while (tmp)
    {
		if (!tmp->argv[0])
			return (FALSE);
        if (tmp->next && pipe(pipefd) < 0)
        {
            perror("pipe");
            return (FALSE);
        }
        pid = fork();
        if (pid < 0)
        {
            perror("fork");
            return (FALSE);
        }
        if (pid == 0) // Child process
        {
            if (tmp->hdoc && tmp->hdoc->del)
            {
                input_fd = handle_heredoc(tmp, shell);
				if (dup2(input_fd, STDIN_FILENO) < 0)
				{
					perror("minishell: dup2 heredoc");
					return (FALSE);
				}
                close(input_fd);
				tmp->hdoc->del = NULL;
            }
			else if (input_fd != -1)
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
            if (input_fd != -1)
                close(input_fd);

            if (tmp->next)
                close(pipefd[1]);

            input_fd = pipefd[0];
            waitpid(pid, &shell->exit_status, 0);
        }
    	tmp = tmp->next;
    }
    return (TRUE);
}
