/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 12:04:43 by nrasamim          #+#    #+#             */
/*   Updated: 2025/02/03 23:59:18 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*resolve_cmd_path(t_shell *shell, char *cmd)
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
		cmd_path = ft_strjoin3(paths[i], "/", cmd);
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

int	other_cmd_without_pipe(t_shell *shell, t_cmd *cmd, int stdin, int stdout)
{
	pid_t	pid;

	setup_signal();
	pid = fork();
	if (pid < 0)
	{
		perror("fork in what_cmd");
		g_global()->is_runing = 0;
		return (1);
	}
	if (pid == 0)
	{
		g_global()->is_runing = 1;
		setup_signal();
		g_global()->exit_status = other_cmd_with_pipe(shell, cmd);
		ft_free_all(shell);
		if (stdin != -1)
			close(stdin);
		if (stdin != -1)
			close(stdout);
		exit(g_global()->exit_status);
	}
	else
		handler_signal_fork(pid);
	return (g_global()->exit_status);
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
	char 		*cmd_path;
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

int	other_cmd_with_pipe(t_shell *shell, t_cmd *cmd)
{
	char	**envp;

	g_global()->exit_status = handle_dot_cmd(shell, cmd);
	if (g_global()->exit_status)
		return (g_global()->exit_status);
	if (!ft_strcmp(cmd->argv[0], ""))
	{
		ft_putstr_fd("''", 2);
		ft_putstr_fd(": command not found\n", 2);
		return (127);
	}
	envp = convert_env_to_array(shell->envp);
	if (!envp)
		return (1);
	if (ft_strchr(cmd->argv[0], '/'))
		return (handle_absolute_cmd(cmd, envp));
	else
		return (handle_relatif_cmd(shell, cmd, envp));
	ft_free_arr(envp);
	return (0);
}

void	what_cmd_without_pipe(t_shell *shell, t_cmd *cmd, int stdin, int stdout)
{
	if (!cmd->argv[0])
		return ;
	g_global()->shell = shell;
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
		g_global()->exit_status = other_cmd_without_pipe(shell, cmd, stdin,
				stdout);
}

void	what_cmd_with_pipe(t_shell *shell, t_cmd *cmd, int stdin, int stdout)
{
	if (!cmd->argv[0])
		return ;
	g_global()->shell = shell;
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
		g_global()->exit_status = other_cmd_with_pipe(shell, cmd);
}
