/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 12:04:43 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/24 10:54:06 by nrasamim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_free_arr(char **arr)
{
	int	i;

	i = 0;
	if (!arr | !*arr)
		return ;
	while (arr[i])
	{
		ft_free(arr[i]);
		i++;
	}
	ft_free(arr);
}

char	*resolve_cmd_path(t_shell *shell, t_cmd *cmd)
{
	char	*path;
	char	**paths;
	int		i;
	char	*cmd_path;

	path = ft_get_env_value(shell->envp, "$PATH");
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	i = 0;
	while (paths[i])
	{
		cmd_path = ft_strjoin3(paths[i], "/", cmd->argv[0]);
		if (access(cmd_path, X_OK | F_OK) == 0)
		{
			ft_free_arr(paths);
			return (cmd_path);
		}
		ft_free(cmd_path);
		i++;
	}
	ft_free_arr(paths);
	return (NULL);
}

int	other_cmd(t_shell *shell, t_cmd *cmd)
{
	char	*cmd_path;
	char	**envp;

	envp = convert_env_to_array(shell->envp);
	if (!envp)
		return (1);
	if (ft_strchr(cmd->argv[0], '/'))
	{
		if (execve(cmd->argv[0], cmd->argv, envp) == -1)
		{
			ft_free_arr(envp);
			perror(cmd->argv[0]);
			return (1);
		}
	}
	cmd_path = resolve_cmd_path(shell, cmd);
	if (!cmd_path)
	{
		ft_free_arr(envp);
		ft_putstr_fd("command not found: ", STDERR_FILENO);
		ft_putstr_fd(cmd->argv[0], STDERR_FILENO);
		write(2, "\n", 1);
		exit(127);
	}
	if (execve(cmd_path, cmd->argv, envp) == -1)
	{
		ft_free_arr(envp);
		ft_free(cmd_path);
		perror(cmd->argv[0]);
		return (1);
	}
	ft_free_arr(envp);
	ft_free(cmd_path);
	return (g_global()->exit_status);
}

void	what_cmd(t_shell *shell, t_cmd *cmd, int stdin, int stdout)
{
	if (cmd->argv[0] == NULL)
		return ;
	if (!ft_strcmp("echo", cmd->argv[0]))
		g_global()->exit_status = ft_echo(cmd->argv);
	else if (!ft_strcmp("pwd", cmd->argv[0]))
		g_global()->exit_status = ft_pwd();
	else if (!ft_strcmp("export", cmd->argv[0]))
		g_global()->exit_status = ft_export(shell, cmd);
	else if (!ft_strcmp("env", cmd->argv[0]))
		g_global()->exit_status = ft_env(shell, cmd);
	else if (!ft_strcmp("unset", cmd->argv[0]))
		g_global()->exit_status = ft_unset(shell, cmd);
	else if (!ft_strcmp("cd", cmd->argv[0]))
		g_global()->exit_status = ft_cd(cmd, shell->envp);
	else if (!ft_strcmp("exit", cmd->argv[0]))
		g_global()->exit_status = ft_exit(shell, cmd->argv, stdin, stdout);
	else
		g_global()->exit_status = other_cmd(shell, cmd);
}
