/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 08:43:12 by frahenin          #+#    #+#             */
/*   Updated: 2025/01/29 12:43:30 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	print_env(t_env *envp)
{
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
		return (1);
	}
	print_env(shell->envp);
	return (0);
}