/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:21:47 by nrasamim          #+#    #+#             */
/*   Updated: 2025/02/01 12:25:49 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*ft_getcwd(char *s)
{
	t_env	*envp;

	envp = g_global()->shell->envp;
	return (ft_get_env_value(envp, s));
}

int	ft_pwd(void)
{
	char	*cwd;

	cwd = ft_getcwd("$PWD");
	if (cwd)
	{
		printf("%s\n", cwd);
		return (0);
	}
	else
		perror("pwd");
	return (1);
}
