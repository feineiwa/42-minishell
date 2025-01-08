/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:51:40 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/08 16:27:49 by nrasamim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

bool	is_valid_cmd(char *cmd)
{
	if (!cmd)
		return (false);
	if (!ft_strcmp("echo", cmd) || !ft_strcmp("cd", cmd) \
	|| !ft_strcmp("pwd", cmd) || !ft_strcmp("export", cmd) \
	|| !ft_strcmp("unset", cmd) || !ft_strcmp("env", cmd) \
	|| !ft_strcmp("exit", cmd))
		return (true);
	return (false);
}

void	what_cmd(t_shell *shell, t_cmd *cmd)
{
	if (!ft_strcmp("echo", cmd->argv[0]))
		shell->exit_status = ft_echo(cmd->argv);
	else if (!ft_strcmp("pwd", cmd->argv[0]))
		shell->exit_status = ft_pwd();
}

bool    launch_cmd(t_shell *shell, t_cmd *cmd)
{
	int input_fd;
    int output_fd;
	int saved_stdin;
    int saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
    saved_stdout = dup(STDOUT_FILENO);
	input_fd = -1;
    if (cmd->input_file)
    {
        input_fd = open(cmd->input_file, O_RDONLY);
        if (input_fd < 0)
        {
            perror("minishell: input_file");
            return (false);
        }
        if (dup2(input_fd, STDIN_FILENO) < 0)
        {
            perror("minishell: dup2 input");
            close(input_fd);
            return (false);
        }
    }
	output_fd = -1;
    if (cmd->output_file)
    {
        int flags = O_WRONLY | O_CREAT;
		if (cmd->append)
        	flags |= O_APPEND;
		else
			flags |= O_TRUNC;
        output_fd = open(cmd->output_file, flags, 0644);
        if (output_fd < 0)
        {
            perror("minishell: output_file");
            if (input_fd != -1)
                close(input_fd);
            return (false);
        }
        if (dup2(output_fd, STDOUT_FILENO) < 0)
        {
            perror("minishell: dup2 output");
            close(output_fd);
            if (input_fd != -1)
                close(input_fd);
            return (false);
        }
    }
	what_cmd(shell, cmd);
	if (input_fd != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
        close(input_fd);
	}
    if (output_fd != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
        close(output_fd);
	}
	return (true);
}
