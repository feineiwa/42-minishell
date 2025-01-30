/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:51:40 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/30 14:22:17 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	save_fds_standart(int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
}

static t_bool	retore_fds_standart(int input_fd, int output_fd, int *stdin,
		int *stdout)
{
	if (input_fd != -1)
	{
		if (dup2(*stdin, STDIN_FILENO) < 0)
		{
			perror("minishell: dup2 input");
			close(input_fd);
			return (FALSE);
		}
		close(input_fd);
	}
	if (output_fd != -1)
	{
		if (dup2(*stdout, STDOUT_FILENO) < 0)
		{
			perror("minishell: dup2 output");
			close(output_fd);
			return (FALSE);
		}
		close(output_fd);
	}
	close(*stdin);
	close(*stdout);
	return (TRUE);
}

static int	setup_redirections(t_cmd *cmd, int *input_fd, int *output_fd)
{
	if (cmd->error_file && !handler_error_flag(cmd, input_fd, output_fd))
		return (1);
	if (cmd->input_file)
	{
		*input_fd = handler_input_redirection(cmd->input_file);
		if (!*input_fd)
			return (1);
	}
	if (cmd->output_file)
	{
		*output_fd = handler_output_redirection(cmd, *input_fd);
		if (!*output_fd)
			return (1);
	}
	return (0);
}

int	launch_cmd(t_shell *shell, t_cmd *cmd, int use_pipe)
{
	int	saved_stdin;
	int	saved_stdout;
	int	input_fd;
	int	output_fd;

	input_fd = -1;
	output_fd = -1;
	save_fds_standart(&saved_stdin, &saved_stdout);
	if (cmd->hdoc && !use_pipe)
	{
		input_fd = handle_heredoc(cmd, shell);
		if (input_fd == -2)
			return (g_global()->exit_status);
		if (input_fd != -1 && dup2(input_fd, STDIN_FILENO) < 0)
		{
			perror("minishell: dup2 input");
			close(input_fd);
			return (1);
		}
		close(input_fd);
	}
	if (setup_redirections(cmd, &input_fd, &output_fd))
		return (retore_fds_standart(input_fd, output_fd, &saved_stdin,
				&saved_stdout), 1);
	if (use_pipe)
		what_cmd_with_pipe(shell, cmd, saved_stdin, saved_stdout);
	else
		what_cmd_without_pipe(shell, cmd, saved_stdin, saved_stdout);
	if (!retore_fds_standart(input_fd, output_fd, &saved_stdin, &saved_stdout))
		return (1);
	return (g_global()->exit_status);
}
