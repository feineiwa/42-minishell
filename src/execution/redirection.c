/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:05:55 by nrasamim          #+#    #+#             */
/*   Updated: 2025/02/03 16:54:23 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	handler_input_redirection(char *input_file)
{
	int	fd;

	fd = -1;
	fd = open(input_file, O_RDONLY);
	if (fd < 0)
	{
		perror(input_file);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror(input_file);
		return (-1);
	}
	return (fd);
}

int	handler_output_redirection(t_cmd *cmd)
{
	int	fd;
	int	flags;

	fd = -1;
	flags = O_WRONLY | O_CREAT;
	if (cmd->append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(cmd->output_file, flags, 0644);
	if (fd < 0)
	{
		perror("minishell: output_file");
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("minishell: dup2 output");
		return (-1);
	}
	return (fd);
}

t_bool	handler_error_flag(t_cmd *cmd, int *input_fd, int *output_fd)
{
	if (cmd->flag_err == 1)
	{
		*input_fd = open(cmd->error_file, O_RDONLY);
		if (*input_fd < 0)
			return (perror(cmd->error_file), FALSE);
	}
	else if (cmd->flag_err == 2)
	{
		*output_fd = open(cmd->error_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (*output_fd < 0)
			return (perror(cmd->error_file), FALSE);
	}
	else if (cmd->flag_err == 3)
	{
		*output_fd = open(cmd->error_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (*output_fd < 0)
			return (perror(cmd->error_file), FALSE);
	}
	return (TRUE);
}
