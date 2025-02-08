/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_other_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 16:34:52 by nrasamim          #+#    #+#             */
/*   Updated: 2025/02/07 09:11:24 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*handle_if_pwd_is_bin(char *cmd)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (path)
	{
		if (!ft_strcmp(path, "/usr/bin"))
			return (ft_strjoin("/usr/bin/", cmd));
		return (ft_free(path), NULL);
	}
	return (NULL);
}

int	check_execution(char **envp, char *cmd_path, t_cmd *cmd)
{
	if (access(cmd_path, X_OK) == 0)
	{
		if (execve(cmd_path, cmd->argv, envp) == -1)
		{
			ft_free_arr(envp);
			return (perror(cmd_path), 127);
		}
	}
	else
	{
		ft_free_arr(envp);
		return (perror(cmd_path), 126);
	}
	return (0);
}

int	handle_absolute_cmd(t_cmd *cmd, char **envp)
{
	struct stat	file_stat;

	if (access(cmd->argv[0], F_OK) == -1)
	{
		ft_free_arr(envp);
		return (perror(cmd->argv[0]), 127);
	}
	if (stat(cmd->argv[0], &file_stat) == -1)
	{
		ft_free_arr(envp);
		perror(cmd->argv[0]);
		return (1);
	}
	if (S_ISDIR(file_stat.st_mode))
	{
		ft_free_arr(envp);
		return (print_err(cmd->argv[0], ": Is a directory\n", NULL, 2), 126);
	}
	return (check_execution(envp, cmd->argv[0], cmd));
}

int	handle_relatif_cmd(t_shell *shell, t_cmd *cmd, char **envp)
{
	char		*cmd_path;
	struct stat	file_stat;

	cmd_path = resolve_cmd_path(shell, cmd->argv[0]);
	if (!cmd_path)
	{
		ft_free_arr(envp);
		return (print_err(cmd->argv[0], ": command not found\n", NULL, 2), 127);
	}
	if (stat(cmd_path, &file_stat) == -1)
	{
		ft_free_arr(envp);
		ft_free(cmd_path);
		return (perror(cmd->argv[0]), 0);
	}
	if (S_ISDIR(file_stat.st_mode))
	{
		ft_free_arr(envp);
		ft_free(cmd_path);
		return (print_err(cmd->argv[0], ": Is a directory\n", NULL, 2), 126);
	}
	g_global()->exit_status = check_execution(envp, cmd_path, cmd);
	ft_free(cmd_path);
	return (g_global()->exit_status);
}
