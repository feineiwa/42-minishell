/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dot_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 13:53:35 by nrasamim          #+#    #+#             */
/*   Updated: 2025/02/05 15:12:38 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	check_path_dot_if_exec(char *cmd_path)
{
	if (access(cmd_path, X_OK) == 0)
	{
		print_err("minishell: .: ", cmd_path, ": cannot execute binary file\n",
			2);
		return (ft_free(cmd_path), 126);
	}
	else
	{
		perror(cmd_path);
		return (ft_free(cmd_path), 1);
	}
	return (0);
}

static int	abs_path_for_dot_cmd(t_cmd *cmd)
{
	struct stat	file_stat;

	if (access(cmd->argv[1], F_OK) == -1)
		return (perror(cmd->argv[1]), 1);
	if (stat(cmd->argv[1], &file_stat) == -1)
		return (perror(cmd->argv[1]), 1);
	if (S_ISDIR(file_stat.st_mode))
	{
		print_err("minishell: ", cmd->argv[1], ": Is a directory\n", 2);
		return (1);
	}
	else
	{
		if (access(cmd->argv[1], X_OK) == 0)
		{
			print_err("minishell: .: ", cmd->argv[1],
				": cannot execute binary file\n", 2);
			return (126);
		}
		else
			return (perror(cmd->argv[1]), 1);
	}
	return (0);
}

static int	relatif_path_for_dot_cmd(t_shell *shell, t_cmd *cmd)
{
	struct stat	file_stat;
	char		*cmd_path;

	cmd_path = resolve_cmd_path(shell, cmd->argv[1]);
	if (cmd_path)
	{
		if (access(cmd_path, F_OK) == -1)
		{
			perror(cmd_path);
			return (ft_free(cmd_path), 1);
		}
		if (stat(cmd_path, &file_stat) == -1)
		{
			perror(cmd_path);
			return (ft_free(cmd_path), 1);
		}
		if (S_ISDIR(file_stat.st_mode))
		{
			print_err("minishell: ", cmd_path, ": Is a directory\n", 2);
			return (ft_free(cmd_path), 1);
		}
		else
			return (check_path_dot_if_exec(cmd_path));
	}
	return (0);
}

int	handle_dot_cmd(t_shell *shell, t_cmd *cmd)
{
	if (!ft_strcmp(cmd->argv[0], "."))
	{
		if (!cmd->argv[1])
			return (print_err("minishell: .: filename argument required\n",
					NULL, NULL, 2), 2);
		if (is_only_dot_or_slash(cmd->argv[1]))
		{
			print_err("minishell: ", ".", ": ", 2);
			print_err(cmd->argv[1], ": is a directory\n", NULL, 2);
			return (1);
		}
		if (ft_strchr(cmd->argv[1], '/'))
			return (abs_path_for_dot_cmd(cmd));
		else
			return (relatif_path_for_dot_cmd(shell, cmd));
	}
	return (0);
}
