/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:51:40 by nrasamim          #+#    #+#             */
/*   Updated: 2025/02/04 19:17:56 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	save_fds_standart(int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
}

t_bool	retore_fds_standart(int input_fd, int output_fd, int stdin, int stdout)
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
	if (stdin != -1)
		close(stdin);
	if (stdin != -1)
		close(stdout);
	return (TRUE);
}

int	setup_redirections(t_cmd *cmd, int *in_fd, int *out_fd)
{
	if (cmd->error_file)
	{
		if (handler_error_flag(cmd, in_fd, out_fd) == FALSE)
			return (1);
	}
	if (cmd->input_file)
	{
		*in_fd = handler_input_redirection(cmd->input_file);
		if (*in_fd != -1)
		{
			if (dup2(*in_fd, STDIN_FILENO) < 0)
			{
				perror("dup2 input");
				return (1);
			}
			close(*in_fd);
			*in_fd = -1;
		}
	}
	if (cmd->output_file)
	{
		*out_fd = handler_output_redirection(cmd);
		if (*out_fd != -1)
		{
			if (dup2(*out_fd, STDOUT_FILENO) < 0)
			{
				perror("dup2 input");
				return (1);
			}
			close(*out_fd);
			*out_fd = -1;
		}
	}
	return (0);
}

int	setup_heredoc(t_cmd *cmd, int *in_fd, int std_fds[2])
{
	if (cmd->hdoc && !g_global()->use_pipe)
	{
		*in_fd = handle_heredoc(cmd, g_global()->shell, std_fds);
		if (*in_fd == -2)
			return (g_global()->exit_status);
		if (*in_fd != -1)
		{
			if (dup2(*in_fd, STDIN_FILENO) < 0)
			{
				perror("minishell: dup2 input");
				return (1);
			}
			close(*in_fd);
			*in_fd = -1;
		}
	}
	return (0);
}

int	launch_cmd(t_shell *shell, t_cmd *cmd, int sa_std[2])
{
	int	in_fd;
	int	out_fd;

	in_fd = -1;
	out_fd = -1;
	g_global()->shell = shell;
	if (g_global()->use_pipe == FALSE)
	{
		g_global()->exit_status = setup_heredoc(cmd, &in_fd, sa_std);
		if (g_global()->exit_status)
			return (g_global()->exit_status);
	}
	if (setup_redirections(cmd, &in_fd, &out_fd))
		return (1);
	if (g_global()->use_pipe)
		what_cmd_with_pipe(shell, cmd, sa_std[0], sa_std[1]);
	else
		what_cmd_without_pipe(shell, cmd, sa_std[0], sa_std[1]);
	// dup2(sa_std[0], STDIN_FILENO);
	// close(sa_std[0]);
	// dup2(sa_std[1], STDOUT_FILENO);
	// close(sa_std[1]);
	return (g_global()->exit_status);
}
