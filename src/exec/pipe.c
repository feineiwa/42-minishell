/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 12:38:58 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/27 16:17:22 by nrasamim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_bool	child_process(t_cmd *cmd, int *pipefd, int input_fd)
{
	if (input_fd != -1)
	{
		if (dup2(input_fd, STDIN_FILENO) < 0)
		{
			perror("dup2 pipe");
			return (FALSE);
		}
		close(input_fd);
	}
	if (cmd->next)
	{
		close(pipefd[0]);
		if (dup2(pipefd[1], STDOUT_FILENO) < 0)
		{
			perror("dup2");
			return (FALSE);
		}
		close(pipefd[1]);
	}
	return (TRUE);
}

t_bool  config_with_pipe(t_shell *shell, t_cmd *cmd)
{
    int     pipefd[2];
    int     input_fd;
    pid_t   *pid;
    t_cmd   *tmp;
    int     *hdoc_fd;
    int     i;
	int		j;
    int     status;

    hdoc_fd = handle_heredoc_with_pipe(cmd, shell);
    if (!hdoc_fd)
        return (FALSE);
    input_fd = -1;
    pid = (pid_t *)malloc(sizeof(pid_t) * ft_cmdsize(cmd));
    if (pid == NULL)
	{
        perror("malloc");
        free(hdoc_fd);
        return (FALSE);
    }
	i = 0;
    tmp = cmd;
    while (cmd)
    {
        if (cmd->next && pipe(pipefd) < 0)
        {
            perror("pipe");
            free(hdoc_fd);
            free(pid);
            return (FALSE);
        }
        pid[i] = fork();
        if (pid[i] < 0)
        {
            perror("fork");
            free(hdoc_fd);
            free(pid);
            return (FALSE);
        }
        if (pid[i] == 0)
        {
            g_global()->is_runing = 1;
            setup_signal();
            if (cmd->hdoc && cmd->hdoc->del)
                input_fd = hdoc_fd[i];
            if (!child_process(cmd, pipefd, input_fd) || !launch_cmd(shell, cmd))
            {
                free(hdoc_fd);
                free(pid);
                exit(1);
            }
            exit(g_global()->exit_status);
        }
        else
        {
            g_global()->is_runing = 2;
            setup_signal();
            if (input_fd != -1 && input_fd != hdoc_fd[i])
                close(input_fd);
            if (cmd->next)
            {
                close(pipefd[1]);
                input_fd = pipefd[0];
            }
            else
                input_fd = -1;
            if (hdoc_fd[i] != -1)
                close(hdoc_fd[i]);
        }
        cmd = cmd->next;
        i++;
    }
    j = 0;
	while(tmp)
	{
		waitpid(pid[j], &status, 0);
		if (WIFEXITED(status))
        {
			g_global()->exit_status = WEXITSTATUS(status);
            if (!ft_strcmp("exit", tmp->argv[0]))
            {
                if (j == 0 && tmp->next)
                {
                    tmp = tmp->next;
                    j++;
                    continue ;
                }
                free(hdoc_fd);
                free(pid);
                return (EXIT);
            }
        }
		else if (WIFSIGNALED(status))
		{
			g_global()->exit_status = 128 + WTERMSIG(status);
			if (WTERMSIG(status) == SIGQUIT)
				write(STDERR_FILENO, "Quit (core dumped)\n", 20);
			else if (WTERMSIG(status) == SIGINT)
				ft_putchar_fd('\n', STDERR_FILENO);
			break ;
		}
		else
			g_global()->exit_status = 1;
        tmp = tmp->next;
		j++;
    }
    while (wait(NULL) > 0)
        ;
    free(hdoc_fd);
    free(pid);
    return (TRUE);
}
