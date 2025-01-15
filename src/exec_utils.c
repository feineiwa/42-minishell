/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:44:26 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/15 12:46:04 by nrasamim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
		if (access(cmd_path, X_OK) == 0)
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
	pid_t	pid;
	char	*cmd_path;
	char	**envp;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork in what_cmd");
		shell->exit_status = 1;
		return (1);
	}
	if (pid == 0)
	{
		cmd_path = resolve_cmd_path(shell, cmd);
		if (!cmd_path)
			return (1);
		envp = convert_env_to_array(shell->envp);
		if (!envp)
		{
			ft_free(cmd_path);
			ft_free_arr(envp);
			return (1);
		}
		if (execve(cmd_path, cmd->argv, envp) == -1)
		{
			perror(cmd->argv[0]);
			return (1);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		else
			shell->exit_status = 1;
	}
	return (0);
}

t_bool	is_valid_cmd(char *cmd)
{
	if (!cmd)
		return (FALSE);
	if (!ft_strcmp("echo", cmd) || !ft_strcmp("cd", cmd) || !ft_strcmp("pwd",
			cmd) || !ft_strcmp("export", cmd) || !ft_strcmp("unset", cmd)
		|| !ft_strcmp("env", cmd) || !ft_strcmp("exit", cmd))
		return (TRUE);
	return (FALSE);
}

void	what_cmd(t_shell *shell, t_cmd *cmd)
{
	if (!ft_strcmp("echo", cmd->argv[0]))
		shell->exit_status = ft_echo(cmd->argv);
	else if (!ft_strcmp("pwd", cmd->argv[0]))
		shell->exit_status = ft_pwd();
	else if (!ft_strcmp("export", cmd->argv[0]))
		shell->exit_status = ft_export(shell, cmd);
	else if (!ft_strcmp("env", cmd->argv[0]))
		shell->exit_status = ft_env(shell, cmd);
	else if (!ft_strcmp("unset", cmd->argv[0]))
		shell->exit_status = ft_unset(shell, cmd);
	else if (!ft_strcmp("cd", cmd->argv[0]))
		shell->exit_status = ft_cd(cmd->argv[1], shell->envp);
    else if (!ft_strcmp("exit", cmd->argv[0]))
		shell->exit_status = ft_exit(shell, cmd->argv);
	else
		shell->exit_status = other_cmd(shell, cmd);
}
