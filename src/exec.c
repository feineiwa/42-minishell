/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:51:40 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/15 13:55:06 by nrasamim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	save_fd_standart(int *saved_stdin, int *saved_stdout)
{
    *saved_stdin = dup(STDIN_FILENO);
    *saved_stdout = dup(STDOUT_FILENO);
}

static void	restore_fd_standart(int stdin, int stdout, int input, int output)
{
    if (input != -1)
	{
        if (dup2(stdin, STDIN_FILENO) < 0)
        {
            perror("minishell: dup2 input");
            close(input);
            return ;
        }
        close(input);
    }
    if (output != -1)
	{
        if (dup2(stdout, STDOUT_FILENO) < 0)
        {
            perror("minishell: dup2 output");
            close(output);
            return ;
        }
        close(output);
    }
}

static int	handle_input_redirection(t_cmd *cmd)
{
	int	input_fd;

	input_fd = -1;
    if (cmd->hdoc->del)
        input_fd = handle_heredoc(cmd);
	if (cmd->input_file)
	{
        input_fd = open(cmd->input_file, O_RDONLY);
        if (input_fd < 0)
		{
            perror("minishell: input_file");
            return (-1);
        }
        if (dup2(input_fd, STDIN_FILENO) < 0)
		{
            perror("minishell: dup2 input");
            close(input_fd);
            return (-1);
        }
    }
    return (input_fd);
}

static int handle_output_redirection(t_cmd *cmd)
{
    int	output_fd;
    int	flags;

	output_fd = -1;
    if (cmd->output_file) 
	{
        flags = O_WRONLY | O_CREAT;
        if (cmd->append)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
        output_fd = open(cmd->output_file, flags, 0644);
        if (output_fd < 0)
		{
            perror("minishell: output_file");
            return (-1);
        }
        if (dup2(output_fd, STDOUT_FILENO) < 0)
		{
            perror("minishell: dup2 output");
            close(output_fd);
            return (-1);
        }
    }
	return (output_fd);
}

t_bool	launch_cmd(t_shell *shell, t_cmd *cmd)
{
	int	saved_stdin;
	int	saved_stdout;
	int	input_fd;
	int	output_fd;
	
    input_fd = -1;
    output_fd = -1;
	save_fd_standart(&saved_stdin, &saved_stdout);
	if (cmd->hdoc || cmd->input_file)
    {
        input_fd = handle_input_redirection(cmd);
        if (input_fd == -1)
        {
            restore_fd_standart(saved_stdin, saved_stdout, input_fd, output_fd);
            return (FALSE);
        }
    }
    if (cmd->output_file)
    {
        output_fd = handle_output_redirection(cmd);
        if (output_fd == -1)
        {
            restore_fd_standart(saved_stdin, saved_stdout, input_fd, output_fd);
            return (FALSE);
        }
    }
	what_cmd(shell, cmd);
	restore_fd_standart(saved_stdin, saved_stdout, input_fd, output_fd);
	return (TRUE);
}
