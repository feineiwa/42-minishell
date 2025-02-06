/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 11:22:30 by frahenin          #+#    #+#             */
/*   Updated: 2025/02/06 10:27:04 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	change_directory_second(char *path, t_env *envp)
{
	char	*old_pwd;

	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
	{
		perror("pwd");
		return (1);
	}
	if (chdir(path) == -1)
	{
		perror(path);
		return (ft_free(old_pwd), 1);
	}
	if (update_oldpwd(envp, old_pwd))
		return (ft_free(old_pwd), 1);
	return (ft_free(old_pwd), 0);
}

static int	change_directory(char *path, t_env *envp)
{
	char	*old_pwd;
	char	*pwd;

	old_pwd = ft_getcwd("$PWD");
	if (!old_pwd)
		return (change_directory_second(path, envp));
	if (chdir(path) == -1)
	{
		perror(path);
		return (1);
	}
	if (old_pwd)
	{
		pwd = getcwd(NULL, 0);
		if (!pwd)
		{
			perror("getcwd");
			return (1);
		}
		if (update_oldpwd(envp, old_pwd) || update_pwd(envp, pwd))
			return (ft_free(pwd), 1);
	}
	return (ft_free(pwd), 0);
}

static int	ft_get_path_value(t_cmd *cmd, t_env *envp, char **path,
		char **path_to_free)
{
	if (!cmd->argv[1])
	{
		*path = ft_get_env_value(envp, "$HOME");
		if (!*path)
			return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), 1);
	}
	else if (cmd->argv[1][0] == '~')
	{
		*path = ft_get_env_value(envp, "$HOME");
		if (!*path)
			return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), 1);
		*path_to_free = ft_strjoin(*path, cmd->argv[1] + 1);
	}
	else if (cmd->argv[1][0] == '-')
	{
		if (cmd->argv[1][1])
			return (print_err("cd: ", cmd->argv[1], ": invalid option\n", 2),
				2);
		*path = ft_get_env_value(envp, "$OLDPWD");
		if (!*path)
			return (ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2), 1);
	}
	else
		*path = cmd->argv[1];
	return (0);
}

int	ft_cd(t_cmd *cmd, t_env *envp)
{
	char	*path;
	char	*path_to_free;
	int		ret;

	path_to_free = NULL;
	ret = 0;
	if (cmd->argc > 2)
		return (write(STDERR_FILENO, "minishell: cd: too many arguments\n", 35),
			1);
	ret = ft_get_path_value(cmd, envp, &path, &path_to_free);
	if (ret)
		return (ret);
	if (path_to_free)
	{
		if (change_directory(path_to_free, envp))
			return (ft_free(path_to_free), 1);
		ft_free(path_to_free);
	}
	else if (change_directory(path, envp))
		return (1);
	if (cmd->argv[1] && !ft_strcmp(cmd->argv[1], "-"))
		ft_pwd();
	return (0);
}
