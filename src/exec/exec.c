/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:51:40 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/27 11:02:39 by nrasamim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	save_fds_standart(int *stdin, int *stdout)
{
	*stdin = dup(STDIN_FILENO);
	*stdout = dup(STDOUT_FILENO);
}

static t_bool	retore_fds_standart(int input_fd, int output_fd, int stdin,
		int stdout)
{
	if (input_fd != -1)
	{
		if (dup2(stdin, STDIN_FILENO) < 0)
		{
			perror("minishell: dup2 input");
			close(input_fd);
			return (FALSE);
		}
		close(input_fd);
	}
	if (output_fd != -1)
	{
		if (dup2(stdout, STDOUT_FILENO) < 0)
		{
			perror("minishell: dup2 output");
			close(output_fd);
			return (FALSE);
		}
		close(output_fd);
	}
	close(stdin);
	close(stdout);
	return (TRUE);
}

static int	handler_input_redirection(char *input_file)
{
	int	fd;

	fd = open(input_file, O_RDONLY);
	if (fd < 0)
	{
		perror(input_file);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror(input_file);
		close(fd);
		return (-1);
	}
	return (fd);
}

static int	handler_output_redirection(t_cmd *cmd, int input_fd)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (cmd->append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(cmd->output_file, flags, 0644);
	if (fd < 0)
	{
		perror("minishell: output_file");
		if (input_fd != -1)
			close(input_fd);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("minishell: dup2 output");
		close(fd);
		if (input_fd != -1)
			close(input_fd);
		return (-1);
	}
	return (fd);
}

static t_bool	handler_error_flag(t_cmd *cmd, int *input_fd, int *output_fd)
{
	if (cmd->flag_err == 1)
	{
		*input_fd = open(cmd->error_file, O_RDONLY);
		if (*input_fd < 0)
		{
			g_global()->exit_status = 1;
			perror(cmd->error_file);
			return (FALSE);
		}
	}
	else if (cmd->flag_err == 2)
	{
		*output_fd = open(cmd->error_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (*output_fd < 0)
		{
			g_global()->exit_status = 1;
			perror(cmd->error_file);
			return (FALSE);
		}
	}
	else if (cmd->flag_err == 3)
	{
		*output_fd = open(cmd->error_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (*output_fd < 0)
		{
			g_global()->exit_status = 1;
			perror(cmd->error_file);
			return (FALSE);
		}
	}
	close(*input_fd);
	close(*output_fd);
	*input_fd = -1;
	*output_fd = -1;
	return (TRUE);
}

t_bool	launch_cmd(t_shell *shell, t_cmd *cmd)
{
	int	output_fd;
	int	input_fd;
	int	saved_stdin;
	int	saved_stdout;

	save_fds_standart(&saved_stdin, &saved_stdout);
	if (cmd->error_file)
		if (!handler_error_flag(cmd, &input_fd, &output_fd))
			return (FALSE);
	input_fd = -1;
	if (cmd->input_file)
	{
		input_fd = handler_input_redirection(cmd->input_file);
		if (!input_fd)
			return (FALSE);
	}
	output_fd = -1;
	if (cmd->output_file)
	{
		output_fd = handler_output_redirection(cmd, input_fd);
		if (!output_fd)
			return (FALSE);
	}
	what_cmd(shell, cmd, saved_stdin, saved_stdout);
	retore_fds_standart(input_fd, output_fd, saved_stdin, saved_stdout);
		return (FALSE);
	return (TRUE);
}