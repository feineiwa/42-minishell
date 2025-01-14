/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 11:54:03 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/13 13:48:53 by nrasamim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int  update_pwd(t_env *envp)
{
    char	*cwd;
    char    *pwd_update;

	cwd = getcwd(NULL, 0);
    if (!cwd)
    {
        perror("pwd");
        return (1);
    }
    pwd_update = ft_strjoin("PWD=", cwd);
    if (!pwd_update)
    {
        perror("minishell: ft_strjoin");
        free(cwd);
        return (1);
    }
    ft_add_env(&envp, pwd_update);
    free(pwd_update);
    free(cwd);
    return (0);
}

static int  update_oldpwd(t_env *envp, char *old_pwd)
{
    char *oldpwd_update;

    oldpwd_update = ft_strjoin("OLDPWD=", old_pwd);
    if (!oldpwd_update)
    {
        perror("minishell: ft_strjoin");
        return (1);
    }
    ft_add_env(&envp, oldpwd_update);
    free(oldpwd_update);
    return (0);
}

int ft_cd(char *path, t_env *envp)
{
    char *old_pwd;

    old_pwd = getcwd(NULL, 0);
    if (!old_pwd)
    {
        perror("pwd");
        return (1);
    }
    if (chdir(path) == -1)
    {
        perror("cd");
        free(old_pwd);
        return (1);
    }
    if (update_oldpwd(envp, old_pwd) || update_pwd(envp))
    {
        free(old_pwd);
        return (1);
    }
    free(old_pwd);
    return (0);
}
