/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 12:04:43 by nrasamim          #+#    #+#             */
/*   Updated: 2025/02/07 09:10:59 by frahenin         ###   ########.fr       */
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
		return (handle_if_pwd_is_bin(cmd));
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
		cmd_path = NULL;
		i++;
	}
	ft_free_arr(paths);
	return (NULL);
}

static int	other_cmd_with_pipe(t_shell *shell, t_cmd *cmd)
{
	char	**envp;

	g_global()->exit_status = handle_dot_cmd(shell, cmd);
	if (g_global()->exit_status)
		return (g_global()->exit_status);
	if (!ft_strcmp(cmd->argv[0], ".."))
	{
		ft_putstr_fd(cmd->argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		return (127);
	}
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
	return (ft_free_arr(envp), 0);
}

static int	other_cmd_without_pipe(t_shell *shell, t_cmd *cmd, int sa_stdin,
		int sa_stdout)
{
	pid_t	pid;
	int		sa_std[2];

	setup_signal();
	sa_std[0] = sa_stdin;
	sa_std[1] = sa_stdout;
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
		restore_standard(sa_std);
		exit(g_global()->exit_status);
	}
	else
		handler_signal_fork(pid);
	return (g_global()->exit_status);
}

void	what_cmd_without_pipe(t_shell *shell, t_cmd *cmd, int sa_stdin,
		int sa_stdout)
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
		g_global()->exit_status = ft_exit(shell, cmd->argv, sa_stdin,
			sa_stdout);
	else
		g_global()->exit_status = other_cmd_without_pipe(shell, cmd, sa_stdin,
			sa_stdout);
}

void	what_cmd_with_pipe(t_shell *shell, t_cmd *cmd, int sa_stdin,
		int sa_stdout)
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
		g_global()->exit_status = ft_exit(shell, cmd->argv, sa_stdin,
			sa_stdout);
	else
		g_global()->exit_status = other_cmd_with_pipe(shell, cmd);
}
