/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 08:43:12 by frahenin          #+#    #+#             */
/*   Updated: 2025/01/27 11:02:39 by nrasamim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	print_env(t_env *envp)
{
	if (!envp)
		return ;
	while (envp)
	{
		if (envp->value)
		{
			ft_putstr_fd(envp->key, 1);
			ft_putchar_fd('=', 1);
			ft_putstr_fd(envp->value, 1);
			ft_putchar_fd('\n', 1);
		}
		envp = envp->next;
	}
}

int	ft_env(t_shell *shell, t_cmd *cmd)
{
	if (cmd->argv[1])
	{
		write (STDERR_FILENO, "env: too many arguments\n", 24);
		return (127);
	}
	print_env(shell->envp);
	return (0);
}