/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:21:47 by nrasamim          #+#    #+#             */
/*   Updated: 2025/02/05 15:12:32 by frahenin         ###   ########.fr       */
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
	char	*cwd_copy;

	cwd = ft_getcwd("$PWD");
	if (!cwd)
	{
		cwd_copy = getcwd(NULL, 0);
		if (cwd_copy)
		{
			printf("%s\n", cwd_copy);
			return (ft_free(cwd_copy), 0);
		}
	}
	if (cwd)
	{
		printf("%s\n", cwd);
		return (0);
	}
	else
		ft_putstr_fd("cannot see pwd\n", 2);
	return (1);
}
