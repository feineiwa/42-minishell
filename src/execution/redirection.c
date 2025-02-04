/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:05:55 by nrasamim          #+#    #+#             */
/*   Updated: 2025/02/04 20:11:42 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	handler_input_redirection(char *input_file, int *in_fd)
{
	*in_fd = open(input_file, O_RDONLY);
	if (*in_fd < 0)
	{
		perror(input_file);
		return (1);
	}
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
	return (0);
}

int	handler_output_redirection(t_cmd *cmd, int *out_fd)
{
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (cmd->append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	*out_fd = open(cmd->output_file, flags, 0644);
	if (*out_fd < 0)
	{
		perror("minishell: output_file");
		return (1);
	}
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
	return (0);
}

t_bool	handler_error_flag(t_cmd *cmd, int *input_fd, int *output_fd)
{
	if (cmd->flag_err == 1)
	{
		*input_fd = open(cmd->error_file, O_RDONLY);
		if (*input_fd < 0)
			return (perror(cmd->error_file), FALSE);
		else
			close(*input_fd);
	}
	else if (cmd->flag_err == 2)
	{
		*input_fd = open(cmd->error_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (*input_fd < 0)
			return (perror(cmd->error_file), FALSE);
		else
			close(*input_fd);
	}
	else if (cmd->flag_err == 3)
	{
		*output_fd = open(cmd->error_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (*output_fd < 0)
			return (perror(cmd->error_file), FALSE);
		else
			close(*output_fd);
	}
	return (TRUE);
}
