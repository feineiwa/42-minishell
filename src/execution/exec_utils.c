/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 12:04:43 by nrasamim          #+#    #+#             */
/*   Updated: 2025/02/02 22:35:57 by frahenin         ###   ########.fr       */
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

int	is_only_dot_or_slash(char *s)
{
	if (!s)
		return (1);
	while (*s)
	{
		if (*s != '.' && *s != '/')
			return (0);
		s++;
	}
	return (1);
}

int	handle_dot_cmd(t_shell *shell, t_cmd *cmd)
{
	struct stat	file_stat;
	char		*cmd_path;

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
		{
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
			}
		}
		else
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
				ft_free(cmd_path);
				return (1);
			}
			else
			{
				if (access(cmd_path, X_OK) == 0)
				{
					print_err("minishell: .: ", cmd_path,
						": cannot execute binary file\n", 2);
					ft_free(cmd_path);
					return (126);
				}
			}
		}
	}
	return (0);
}

int	other_cmd_with_pipe(t_shell *shell, t_cmd *cmd)
{
	char	*cmd_path;
	char	**envp;
	struct stat	file_stat;

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
	{
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
		if (access(cmd->argv[0], X_OK) == 0)
		{
			if (execve(cmd->argv[0], cmd->argv, envp) == -1)
			{
				ft_free_arr(envp);
				return (perror(cmd->argv[0]), 127);
			}
		}
		else
		{
			ft_free_arr(envp);
			return (perror(cmd->argv[0]), 126);
		}
	}
	else
		cmd_path = resolve_cmd_path(shell, cmd->argv[0]);
	if (!cmd_path)
	{
		ft_free_arr(envp);
		ft_putstr_fd(cmd->argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		return (127);
	}
	if (stat(cmd_path, &file_stat) == -1)
	{
		ft_free_arr(envp);
		ft_free(cmd_path);
		perror(cmd->argv[0]);
		return (1);
	}
	if (S_ISDIR(file_stat.st_mode))
	{
		ft_free_arr(envp);
		ft_free(cmd_path);
		return (print_err(cmd->argv[0], ": Is a directory\n", NULL, 2), 126);
	}
	if (access(cmd_path, X_OK) == 0)
	{
		if (execve(cmd_path, cmd->argv, envp) == -1)
		{
			ft_free_arr(envp);
			ft_free(cmd_path);
			return (perror(cmd_path), 127);
		}
		else
		{
			ft_free_arr(envp);
			ft_free(cmd_path);
			return (perror(cmd->argv[0]), 126);
		}
	}
	ft_free_arr(envp);
	ft_free(cmd_path);
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
