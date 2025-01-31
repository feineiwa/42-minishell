/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 11:22:30 by frahenin          #+#    #+#             */
/*   Updated: 2025/01/31 09:08:29 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	update_pwd(t_env *envp)
{
	char	*cwd;
	char	*pwd_update;

	cwd = getcwd(NULL, 0);
	pwd_update = ft_strjoin("PWD=", cwd);
	if (!pwd_update)
	{
		perror(pwd_update);
		ft_free(cwd);
		return (1);
	}
	ft_add_env(&envp, pwd_update);
	ft_free(pwd_update);
	ft_free(cwd);
	return (0);
}

static int	update_oldpwd(t_env *envp, char *old_pwd)
{
	char	*old_pwd_update;

	old_pwd_update = ft_strjoin("OLDPWD=", old_pwd);
	if (!old_pwd_update)
	{
		perror(old_pwd_update);
		return (1);
	}
	ft_add_env(&envp, old_pwd_update);
	ft_free(old_pwd_update);
	return (0);
}

static int	change_directory(char *path, t_env *envp)
{
	char	*old_pwd;

	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
	{
		perror("getcwd");
		return (1);
	}
	if (chdir(path) == -1)
	{
		perror(path);
		ft_free(old_pwd);
		return (1);
	}
	if (update_oldpwd(envp, old_pwd) || update_pwd(envp))
	{
		ft_free(old_pwd);
		return (1);
	}
	ft_free(old_pwd);
	return (0);
}

int	ft_get_path_value(t_cmd *cmd, t_env *envp, char **path, char **path_to_free)
{
	if (!cmd->argv[1])
		*path = ft_get_env_value(envp, "$HOME");
	else if (cmd->argv[1][0] == '~')
		*path_to_free = ft_strjoin(ft_get_env_value(envp, "$HOME"), cmd->argv[1]
				+ 1);
	else if (cmd->argv[1][0] == '-')
	{
		if (cmd->argv[1][1])
			return (print_err("cd: ", cmd->argv[1], ": invalid option\n", 2),
				2);
		*path = ft_get_env_value(envp, "$OLDPWD");
	}
	else
		*path = cmd->argv[1];
	return (0);
}

int	ft_cd(t_cmd *cmd, t_env *envp)
{
	char	*path;
	char	*path_to_free;

	path_to_free = NULL;
	if (cmd->argc > 2)
		return (write(STDERR_FILENO, "minishell: cd: too many arguments\n", 35),
			1);
	if (ft_get_path_value(cmd, envp, &path, &path_to_free))
		return (2);
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
